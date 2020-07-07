#include "journey_list.h"

JourneyList::JourneyList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->db = db;
    
    setLayout(this->layout);
    
    this->scroll_area->setWidgetResizable(true);
    
    QPushButton *button_new_journey = new QPushButton("new journey");
    connect(button_new_journey, &QPushButton::clicked, this, &JourneyList::journeyNew);
    
    this->layout->addWidget(this->scroll_area);
    this->layout->addWidget(button_new_journey);
}

void JourneyList::showJourneys()
{
    QList<QMap<QString,QVariant>> data = this->db->selectJourneys();
    QString currency_code_default = this->db->currencySelectDefault()["code"].toString();
    
    this->grid = new QGridLayout(this);
    this->scroll_widget = new QWidget(this);
    this->scroll_widget->setLayout(this->grid);
    this->scroll_area->setWidget(this->scroll_widget);
    
    // add headers
    this->grid->addWidget(new  QLabel("<b>journey name</b>"), 0, 2);
    this->grid->addWidget(new  QLabel("<b>date from</b>"), 0, 3, Qt::AlignCenter);
    this->grid->addWidget(new  QLabel("<b>date to</b>"), 0, 4, Qt::AlignCenter);
    this->grid->addWidget(new  QLabel("<b>total costs</b>"), 0, 5, Qt::AlignRight);
    
    QIcon icon_edit = QIcon::fromTheme("document-properties");
    QIcon icon_delete = QIcon::fromTheme("edit-delete");
    int current_year = 0;
    bool date_already_inserted = false;
    int row = 1;
    for (int i=1; i < data.length()+1; ++i)
    {
        // for running from 1 -> ...+1 because of having the headers at line 0 in the grid
        // because of this, we need to -1 here for getting the iterating and the further positions in the grid right
        QMap<QString,QVariant> journey = data.at(i-1);
        ++row;
        //row = i;
        
        qlonglong rowid = journey["rowid"].toLongLong();
        QString name = journey["name"].toString();
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(icon_edit);
        button_edit->setMaximumWidth(40);
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, name]{ JourneyList::journeyEdit(rowid, name); });
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(icon_delete);
        button_delete->setMaximumWidth(40);
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ JourneyList::journeyDelete(rowid, name); });
        
        QDate date_now = QDate::currentDate();
        QDate date_from_ = QDate::fromString(journey["date_from"].toString(), "yyyy-MM-dd");
        QString date_from = date_from_.toString();
        QString date_to = QDate::fromString(journey["date_to"].toString(), "yyyy-MM-dd").toString();
        
        int year = QDate::fromString(journey["date_from"].toString(), "yyyy-MM-dd").year();
        if (current_year != year)
        {
            current_year = year;
            this->grid->addWidget(new QLabel("<i><b>"+QString::number(year)+"</i></b>"), row, 2);
            ++row;
        }
        
        if (!date_already_inserted && (date_from_ <= date_now))
        {
            this->grid->addWidget(new QLabel("<b><u>today</u></b>"), row, 2);
            this->grid->addWidget(new QLabel("<b><u>"+date_now.toString()+"</b></u>"), row, 3, Qt::AlignRight);
            date_already_inserted = true;
            ++row;
        }
        
        
        this->grid->addWidget(button_delete, row, 0);
        this->grid->addWidget(button_edit, row, 1);
        this->grid->addWidget(new QLabel(name), row, 2);
        this->grid->addWidget(new QLabel(date_from), row, 3, Qt::AlignRight);
        this->grid->addWidget(new QLabel(date_to), row, 4, Qt::AlignRight);
        this->grid->addWidget(new QLabel(journey["costs"].toString()), row, 5, Qt::AlignRight);
        this->grid->addWidget(new QLabel(currency_code_default), row, 6);
    }
    // push all columns to the left for getting the table a bit more compact
    this->grid->setColumnStretch(100, 100);
    // push everything up
    this->grid->setRowStretch(data.length()+100, 100);
}

void JourneyList::journeyNew()
{
    //emit signalJourneyNew();
    
    qlonglong rowid = this->db->insertJourney("", "", "", "");
    emit signalJourneyEdit(rowid, "");
}

void JourneyList::journeyEdit(qlonglong rowid, QString name)
{
    emit signalJourneyEdit(rowid, name);
}

void JourneyList::journeyDelete(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Delete Journey "+name, "Really delete Journey \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteJourney(rowid);
        
        refresh();
    }
}

void JourneyList::refresh()
{
    // store the scrollbar position first to be restored in scrollBarRangeChanged afterwards
    this->scrollbar_pos = this->scroll_area->verticalScrollBar()->sliderPosition();

    this->scroll_widget->deleteLater();
    this->grid = nullptr;
    showJourneys();
}

void JourneyList::showEvent(QShowEvent */* event */)
{
    refresh();
}
