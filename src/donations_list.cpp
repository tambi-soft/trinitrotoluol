#include "donations_list.h"

DonationsList::DonationsList(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    
}
