#ifndef STATS_H
#define STATS_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLayoutItem>
#include <QLayout>
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QMap>
#include <QList>
#include <QShowEvent>
#include <QIcon>

#include "db_adapter.h"

#include "math.h"

class Stats : public QWidget
{
    Q_OBJECT
public:
    explicit Stats(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    void addPeopleStats();
    void addMoneyStats();
    void addRemainingStats();
    void clearLayout(QLayout *layout);
    
    /* needed to be able to update these values while the lineedit is edited */
    QLineEdit *edit_money_needed;
    QLabel *label_need_remaining;
    QLabel *label_need_donors;
    
signals:
    
public slots:
    void showEvent(QShowEvent */*event*/);
    /* needed to be able to update these values while the lineedit is edited */
    void onMoneyTargetChanged(QString target);
    void onMoneyTargetCalculate();
};

#endif // STATS_H
