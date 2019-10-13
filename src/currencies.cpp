#include "currencies.h"

Currencies::Currencies(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    
}
