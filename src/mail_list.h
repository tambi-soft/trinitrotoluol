#ifndef MAILLIST_H
#define MAILLIST_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class MailList : public QWidget
{
    Q_OBJECT
public:
    explicit MailList(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    void closeCurrentTabSignal();
    
public slots:
};

#endif // MAILLIST_H
