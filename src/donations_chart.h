#ifndef DONATIONSCHART_H
#define DONATIONSCHART_H


#include <QtCharts>
#include <QLineSeries>
#include <QScatterSeries>
#include <QPointF>
#include <QPen>
#include <QToolTip>
#include <QLabel>
#include <QWheelEvent>

#include "lib/chartview.h"
#include "db_adapter.h"

class DonationsChart : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsChart(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
    QWidget *container_widget = nullptr;
    QLayout *container_layout = nullptr;
    
    QChart *chart = new QChart;
    QScatterSeries *scatter_monthly = nullptr;
    QScatterSeries *scatter_full = nullptr;
    
    QList<QMap<QString,QVariant>> data_monthly;
    QList<QMap<QString,QVariant>> data_full;
    QString currency_code;
    
    QLabel *label_value = new QLabel;
    
    void drawChart();
    
protected:
    
    
private slots:
    void onMonthlyHover(QPointF pos, bool state);
    void onFullHover(QPointF pos, bool state);
    
signals:
    
};

#endif // DONATIONSCHART_H
