#ifndef EXPENSESLIST_H
#define EXPENSESLIST_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDate>
#include <QLabel>

#include "db_adapter.h"
#include "combo_currencies.h"
#include "lib/grid_widget.h"

class ExpensesList : public GridWidget
{
    Q_OBJECT
public:
    explicit ExpensesList(DbAdapter *db, GridWidget *parent = nullptr);
    
    void updateView();
    
private:
    DbAdapter *db;
    
    void showData();
    
signals:
    
public slots:
    void onDeleteButtonClicked(qlonglong rowid, QString name);
    void onEditButtonClicked(qlonglong rowid);
    void onNewButtonClicked();
    void showEvent(QShowEvent */*event*/);
};

class ExpensesEdit : public QWidget
{
    Q_OBJECT
public:
    explicit ExpensesEdit(qlonglong rowid, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid;
    qlonglong rowid_currency;
    DbAdapter *db;
    QVBoxLayout *layout = new QVBoxLayout;
    
    QLineEdit *edit_name = new QLineEdit;
    QLineEdit *edit_amount = new QLineEdit;
    QLineEdit *edit_cost_one = new QLineEdit;
    ComboCurrencies *combo_currency;
    QDateTimeEdit *edit_date = new QDateEdit;
    QPlainTextEdit *edit_notes = new QPlainTextEdit;
    QCheckBox *check_settled = new QCheckBox;
    
signals:
    void signalUpdate();
    
public slots:
    void saveData();
};

#endif // EXPENSESLIST_H
