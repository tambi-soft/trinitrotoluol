#ifndef PEOPLE_LIST_H
#define PEOPLE_LIST_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "db_adapter.h"

class PeopleList : public QWidget
{
    Q_OBJECT
public:
    explicit PeopleList(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    QTabWidget *table_widget = new QTabWidget();
    DbAdapter *db;
    
signals:
    
public slots:
};

#endif // PEOPLE_LIST_H
