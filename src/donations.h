#ifndef DONATIONS_H
#define DONATIONS_H

#include <QObject>
#include <QWidget>

#include <QTabWidget>

#include "db_adapter.h"
#include "donations_list.h"
#include "donations_map_edit.h"

class Donations : public QTabWidget
{
    Q_OBJECT
public:
    explicit Donations(DbAdapter *db, QTabWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    
};





// for reloading the chart, just delete this container and reinit it
// this way is far easier and has lower risks for memory leaks and so on

#include "donations_chart.h"

class DonationsChartContainer : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsChartContainer(DbAdapter *db, QWidget *parent = nullptr);
    
protected:
    void hideEvent(QHideEvent */**event**/);
    void showEvent(QShowEvent */**event**/);
    
private:
    DbAdapter *db;
    
    QWidget *chart = nullptr;
};

#endif // DONATIONS_H
