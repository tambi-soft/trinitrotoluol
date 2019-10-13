#ifndef CURRENCIES_H
#define CURRENCIES_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class Currencies : public QWidget
{
    Q_OBJECT
public:
    explicit Currencies(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    
public slots:
};

#endif // CURRENCIES_H
