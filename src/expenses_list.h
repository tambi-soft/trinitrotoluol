#ifndef EXPENSESLIST_H
#define EXPENSESLIST_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>

#include "db_adapter.h"

class ExpensesList : public QWidget
{
    Q_OBJECT
public:
    explicit ExpensesList(DbAdapter *db, QWidget *parent = nullptr);
    
    void addNew();
    void updateView();
    
private:
    DbAdapter *db;
    QVBoxLayout *layout = new QVBoxLayout;
    QTableWidget *table = new QTableWidget;
    void loadData();
    
signals:
    
public slots:
    void showEvent(QShowEvent */*event*/);
};

#endif // EXPENSESLIST_H
