#ifndef DONATIONSLIST_H
#define DONATIONSLIST_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class DonationsList : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsList(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    
};

#endif // DONATIONSLIST_H
