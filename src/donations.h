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
#include <QScatterSeries>
#include <QPointF>
#include <QPen>
#include <QToolTip>
#include <QLabel>
#include <QWheelEvent>

class DonationsChart : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsChart(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
    QChart *chart = new QChart;
    QScatterSeries *scatter_monthly;
    QScatterSeries *scatter_full;
    
    QList<QMap<QString,QVariant>> data_monthly;
    QList<QMap<QString,QVariant>> data_full;
    QString currency_code;
    
    QLabel *label_value = new QLabel;

    void drawChart();
    
protected:
    void wheelEvent(QWheelEvent *event);
    
private slots:
    void onMonthlyHover(QPointF pos, bool state);
    void onFullHover(QPointF pos, bool state);
    
signals:
    
};





#import <QMessageBox>

#import "lib/grid_widget.h"

class DonationsMapEdit : public GridWidget
{
    Q_OBJECT
public:
    explicit DonationsMapEdit(DbAdapter *db, GridWidget *parent = nullptr);
    
    void showData();
    
private:
    DbAdapter *db;
    
private slots:
    void onDeleteButtonClicked(qlonglong rowid_person, QString name);
    void onEditButtonClicked(QString tnt_name);
};

#endif // DONATIONS_H
