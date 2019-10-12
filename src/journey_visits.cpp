#include "journey_visits.h"

JourneyVisits::JourneyVisits(qlonglong rowid_journey, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table (new QTableWidget)
    , layout (new QVBoxLayout)
{
    this->rowid_journey = rowid_journey;
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_visit = new QPushButton("add visited person");
    connect(button_new_visit, &QPushButton::clicked, this, &JourneyVisits::addNewVisit);
    
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new_visit);
    
    this->table->setColumnCount(3);
    QStringList header;
    header << "name" << "date" << "notes";
    this->table->setHorizontalHeaderLabels(header);
    
    loadData();
}

void JourneyVisits::loadData()
{
    QList<QMap<QString,QVariant>> data = this->db->selectVisitsForJourney(this->rowid_journey);
    this->table->setRowCount(data.length());
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> journey = data.at(i);
        
        this->table->setItem(i, 0, new QTableWidgetItem(journey["name"].toString()));
        this->table->setItem(i, 1, new QTableWidgetItem(QDate::fromString(journey["date"].toString(), "yyyy-MM-dd").toString()));
        this->table->setItem(i, 2, new QTableWidgetItem(journey["notes"].toString()));
    }
    
    this->table->resizeColumnsToContents();
}

void JourneyVisits::addNewVisit()
{
    
}
