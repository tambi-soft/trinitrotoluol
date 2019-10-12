#ifndef JOURNEYVISITS_H
#define JOURNEYVISITS_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QMap>
#include <QList>
#include <QDate>

#include "db_adapter.h"

class JourneyVisits : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyVisits(qlonglong rowid_journey, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_journey;
    DbAdapter *db;
    QTableWidget *table;
    QVBoxLayout *layout;
    
    void loadData();
    
signals:
    
public slots:
    
private slots:
    void addNewVisit();
    
};

#endif // JOURNEYVISITS_H
