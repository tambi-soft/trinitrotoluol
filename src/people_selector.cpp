#include "people_selector.h"

PeopleSelector::PeopleSelector(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->db = db;
    
    setLayout(this->layout);
    
    
}
