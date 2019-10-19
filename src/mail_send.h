#ifndef MAILSEND_H
#define MAILSEND_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class MailSend : public QWidget
{
    Q_OBJECT
public:
    explicit MailSend(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid;
    
signals:
    
public slots:
};

#endif // MAILSEND_H
