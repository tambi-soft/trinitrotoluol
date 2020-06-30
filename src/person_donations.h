#ifndef PERSONDONATIONS_H
#define PERSONDONATIONS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QLabel>

#include "db_adapter.h"
#include "lib/grid_widget.h"

class PersonDonations : public QTabWidget
{
    Q_OBJECT
public:
    explicit PersonDonations(DbAdapter *db, qlonglong rowid_person, QTabWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid_person;
    
};




class PersonDonationsList : public GridWidget
{
    Q_OBJECT
public:
    explicit PersonDonationsList(DbAdapter *db, qlonglong rowid_person, GridWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid_person;
    
    void showData();
    
};





#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QtCharts>
#include <QLineSeries>
#include <QScatterSeries>
#include <QPointF>
#include <QPen>
#include <QToolTip>
#include <QLabel>
#include <QWheelEvent>

#include "lib/chartview.h"

class PersonDonationsChart : public QWidget
{
    Q_OBJECT
public:
    explicit PersonDonationsChart(DbAdapter *db, qlonglong rowid_person, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid_person;
    QString currency_code;
    QChart *chart = new QChart;
    
    QList<QMap<QString,QVariant>> data;
    QScatterSeries *series_scatter;
    QLabel *label_value = new QLabel;
    
    void showData();
    
private slots:
    void onScatterHover(QPointF pos, bool state);
};





#include "combo_currencies.h"

class PersonDonationsPromises : public QWidget
{
    Q_OBJECT
public:
    explicit PersonDonationsPromises(DbAdapter *db, qlonglong rowid_person, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid_person;
    
    QVBoxLayout *layout = new QVBoxLayout;
    
    void onButtonNewClicked();
};

#endif // PERSONDONATIONS_H
