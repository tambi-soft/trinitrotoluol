#ifndef EXPENSESLIST_H
#define EXPENSESLIST_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QTableWidget>
#include <QTableWidgetItem>
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

class ExpensesList : public QWidget
{
    Q_OBJECT
public:
    explicit ExpensesList(DbAdapter *db, QWidget *parent = nullptr);
    
    void updateView();
    
private:
    DbAdapter *db;
    QVBoxLayout *layout = new QVBoxLayout;
    QTableWidget *table = new QTableWidget;
    void loadData();
    
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
    
    QList<QMap<QString,QVariant>> currencies;
    
    QLineEdit *edit_name = new QLineEdit;
    QLineEdit *edit_amount = new QLineEdit;
    QLineEdit *edit_cost_one = new QLineEdit;
    QComboBox *combo_currency = new QComboBox;
    QDateTimeEdit *edit_date = new QDateEdit;
    QPlainTextEdit *edit_notes = new QPlainTextEdit;
    QCheckBox *check_settled = new QCheckBox;
    
signals:
    void signalUpdate();
    
public slots:
    void saveData();
};

#endif // EXPENSESLIST_H
