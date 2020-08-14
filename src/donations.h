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

#include <QtCharts>
#include <QLineSeries>
#include <QScatterSeries>
#include <QPointF>
#include <QPen>
#include <QToolTip>
#include <QLabel>
#include <QWheelEvent>

#include "lib/chartview.h"

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





#include <QMessageBox>
#include <QDialog>

#include "lib/grid_widget.h"
#include "people_selector.h"
#include "lib/growing_text_edit.h"

class DonationsMapEdit : public GridWidget
{
    Q_OBJECT
public:
    explicit DonationsMapEdit(DbAdapter *db, GridWidget *parent = nullptr);
    
    void showData();
    
protected:
    void hideEvent(QHideEvent */**event**/);
    void showEvent(QShowEvent */**event**/);
    
private:
    DbAdapter *db;
    QDialog *dialog_select_person = new QDialog;
    QString tnt_name;
    
    GrowingTextEdit *help;
    
private slots:
    void onDeleteButtonClicked(qlonglong rowid_person, QString name);
    void onEditButtonClicked(QString tnt_name);
    void onPersonSelected(qlonglong rowid, QString name);
};

#endif // DONATIONS_H
