#ifndef DONATIONS_H
#define DONATIONS_H

#include <QObject>
#include <QWidget>

#include <QTabWidget>

#include "db_adapter.h"
#include "donations_list.h"

class Donations : public QTabWidget
{
    Q_OBJECT
public:
    explicit Donations(DbAdapter *db, QTabWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    
};




#include <QtCharts>
#include <QLineSeries>

class DonationsChart : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsChart(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    
};

#endif // DONATIONS_H
