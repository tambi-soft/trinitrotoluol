#ifndef PEOPLESELECTOR_H
#define PEOPLESELECTOR_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include "db_adapter.h"

class PeopleSelector : public QWidget
{
    Q_OBJECT
public:
    explicit PeopleSelector(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    
signals:
    
public slots:
};

#endif // PEOPLESELECTOR_H
