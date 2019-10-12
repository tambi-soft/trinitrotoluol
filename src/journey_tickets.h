#ifndef JOURNEYTICKETS_H
#define JOURNEYTICKETS_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "db_adapter.h"

class JourneyTickets : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyTickets(qlonglong rowid_journey, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_journey;
    DbAdapter *db;
    QTableWidget *table;
    QVBoxLayout *layout;
    
signals:
    
public slots:
    
private slots:
    void addNewTicket();
};

#endif // JOURNEYTICKETS_H
