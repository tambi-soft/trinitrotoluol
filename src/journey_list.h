#ifndef JOURNEYLIST_H
#define JOURNEYLIST_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class JourneyList : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyList(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    
public slots:
};

#endif // JOURNEYLIST_H
