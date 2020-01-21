#ifndef CURRENCIES_LIST_H
#define CURRENCIES_LIST_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QMessageBox>

#include <QVBoxLayout>
#include <QScrollArea>

#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include "db_adapter.h"

class CurrenciesList : public QWidget
{
    Q_OBJECT
public:
    explicit CurrenciesList(DbAdapter *db, QWidget *parent = nullptr);
    
    void showData();
    
private:
    DbAdapter *db;
    
    QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *grid = new QGridLayout;
    
    QScrollArea *scroll_area = new QScrollArea;
    QWidget *scroll_widget = nullptr;
    
    QComboBox *combo_default_currency;
    
signals:
    
public slots:
    void onEditButtonClicked(qlonglong rowid);
    void onNewButtonClicked();
    void onDeleteButtonClicked(qlonglong rowid, QString code);
    void updateView();
};


#include <QLineEdit>
#include <QTextEdit>
class CurrenciesEdit : public QWidget
{
    Q_OBJECT
public:
    explicit CurrenciesEdit(qlonglong rowid, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid;
    DbAdapter *db;
    QVBoxLayout *layout = new QVBoxLayout;
    
    QLineEdit *edit_code = new QLineEdit;
    QLineEdit *edit_exchange = new QLineEdit;
    QTextEdit *edit_notes = new QTextEdit;
    
signals:
    void signalUpdate();
    
public slots:
    void saveData();
    
};

#endif // CURRENCIES_LIST_H
