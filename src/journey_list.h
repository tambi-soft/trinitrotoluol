#ifndef JOURNEYLIST_H
#define JOURNEYLIST_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QPushButton>

#include <QStringList>
#include <QList>
#include <QMap>

#include <QShowEvent>

#include "db_adapter.h"
#include "journey_edit.h"

class JourneyList : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyList(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    QTableWidget *table;
    
    void showJourneys();
    
signals:
    
public slots:
    void showEvent(QShowEvent */*event*/);
};

#endif // JOURNEYLIST_H
