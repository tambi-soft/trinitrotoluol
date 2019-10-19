#include "mail_send.h"

MailSend::MailSend(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
{
    this->db = db;
    this->rowid = rowid;
    
    
}
