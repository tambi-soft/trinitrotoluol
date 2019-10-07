#ifndef JOURNEYEDIT_H
#define JOURNEYEDIT_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class JourneyEdit : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyEdit(DbAdapter *db, QWidget *parent = nullptr);
    explicit JourneyEdit(int rowid, DbAdapter *db, QWidget *parent = nullptr);
    
signals:
    
public slots:
};

#endif // JOURNEYEDIT_H
