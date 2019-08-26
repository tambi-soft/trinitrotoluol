#ifndef PEOPLE_LIST_H
#define PEOPLE_LIST_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>

#include "db_adapter.h"

class PeopleList : public QWidget
{
    Q_OBJECT
public:
    explicit PeopleList(DbAdapter *db, QWidget *parent = nullptr);
    void showPeople();
    
private:
    QTableWidget *table_widget = new QTableWidget();
    DbAdapter *db;
    
signals:
    
public slots:
};

#endif // PEOPLE_LIST_H
