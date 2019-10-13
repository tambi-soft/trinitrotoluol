#include "journey_list.h"

JourneyList::JourneyList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , table (new QTableWidget)
{
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_journey = new QPushButton("new journey");
    connect(button_new_journey, &QPushButton::clicked, this, &JourneyList::journeyNew);
    
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new_journey);
}

void JourneyList::showJourneys()
{
    QList<QMap<QString,QVariant>> data = this->db->selectJourneys();
    
    
    this->table->setRowCount(data.length());
    QStringList labels;
    labels << "" << "name" << "date from" << "date to" << "costs";
    this->table->setColumnCount(labels.length());
    this->table->setHorizontalHeaderLabels(labels);
    
    
    for (int i = 0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> journey = data.at(i);
        
        qlonglong rowid = journey["rowid"].toLongLong();
        QString name = journey["name"].toString();
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setMaximumWidth(40);
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, name]{ JourneyList::journeyEdit(rowid, name); });
        
        QString date_from = QDate::fromString(journey["date_from"].toString(), "yyyy-MM-dd").toString();
        QString date_to = QDate::fromString(journey["date_to"].toString(), "yyyy-MM-dd").toString();
        
        this->table->setCellWidget(i, 0, button_edit);
        
        this->table->setItem(i, 1, new QTableWidgetItem(name));
        this->table->setItem(i, 2, new QTableWidgetItem(date_from));
        this->table->setItem(i, 3, new QTableWidgetItem(date_to));
        
        QTableWidgetItem *costs = new QTableWidgetItem(journey["costs"].toString());
        costs->setData(Qt::TextAlignmentRole,int(Qt::AlignRight|Qt::AlignVCenter));
        this->table->setItem(i, 4, costs);
    }
    
    this->table->resizeColumnsToContents();
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

void JourneyList::showEvent(QShowEvent */* event */)
{
    this->table->clear();
    
    showJourneys();
}
