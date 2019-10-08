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
    labels << "" << "name" << "date from" << "date to";
    this->table->setColumnCount(labels.length());
    this->table->setHorizontalHeaderLabels(labels);
    
    
    for (int i = 0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> journey = data.at(i);
        
        qlonglong rowid = journey["rowid"].toLongLong();
        QString name = journey["name"].toString();
        QPushButton *button_edit = new QPushButton("edit");
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, name]{ JourneyList::journeyEdit(rowid, name); });
        
        this->table->setCellWidget(i, 0, button_edit);
        
        this->table->setItem(i, 1, new QTableWidgetItem(name));
        this->table->setItem(i, 2, new QTableWidgetItem(journey["date_from"].toString()));
        this->table->setItem(i, 3, new QTableWidgetItem(journey["date_to"].toString()));
    }
    
    this->table->resizeColumnsToContents();
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void JourneyList::journeyNew()
{
    emit signalJourneyNew();
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
