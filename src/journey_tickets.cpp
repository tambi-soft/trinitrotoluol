#include "journey_tickets.h"

JourneyTickets::JourneyTickets(qlonglong rowid_journey, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table (new QTableWidget)
    , layout (new QVBoxLayout)
{
    this->rowid_journey = rowid_journey;
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_visit = new QPushButton("add ticket");
    connect(button_new_visit, &QPushButton::clicked, this, &JourneyTickets::addNewTicket);
    
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new_visit);
    
}

void JourneyTickets::addNewTicket()
{
    
}
