#ifndef EXPENSESLIST_H
#define EXPENSESLIST_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class ExpensesList : public QWidget
{
    Q_OBJECT
public:
    explicit ExpensesList(DbAdapter *db, QWidget *parent = nullptr);
    
signals:
    
public slots:
};

#endif // EXPENSESLIST_H
