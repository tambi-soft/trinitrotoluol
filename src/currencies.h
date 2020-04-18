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
#include <QTextEdit>

#include "db_adapter.h"
#include "lib/grid_widget.h"
#include "lib/growing_text_edit.h"

class CurrenciesList : public GridWidget
{
    Q_OBJECT
public:
    explicit CurrenciesList(DbAdapter *db, GridWidget *parent = nullptr);
    
    void showData();
    QTextEdit *showHelp();
    void onUpdateSignaled();
    
private:
    DbAdapter *db;
    
    QComboBox *combo_default_currency;
    
signals:
    
public slots:
    void onEditButtonClicked(qlonglong rowid);
    void onNewButtonClicked();
    void onDeleteButtonClicked(qlonglong rowid, QString code);
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
