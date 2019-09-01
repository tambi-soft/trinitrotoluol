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
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QMap>
#include <QList>
#include <QShowEvent>

#include "db_adapter.h"

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
    void clearLayout(QLayout *layout);
    
signals:
    
public slots:
    void showEvent(QShowEvent */*event*/);
};

#endif // STATS_H