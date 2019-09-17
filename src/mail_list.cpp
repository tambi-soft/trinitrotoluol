#include "mail_list.h"

MailList::MailList(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
}
