#ifndef EXPENSESLIST_H
#define EXPENSESLIST_H

#include <QObject>
#include <QWidget>

class ExpensesList : public QWidget
{
    Q_OBJECT
public:
    explicit ExpensesList(QWidget *parent = nullptr);
    
signals:
    
public slots:
};

#endif // EXPENSESLIST_H
