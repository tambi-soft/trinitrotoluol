#ifndef DONATIONSLIST_H
#define DONATIONSLIST_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QIcon>
#include <QDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

#include "db_adapter.h"

class DonationsList : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsList(DbAdapter *db, QWidget *parent = nullptr);

    void showData();
    void recreateView();
    
protected:
    void hideEvent(QHideEvent */**event**/);
    void showEvent(QShowEvent */**event**/);
    
private:
    void donationDelete(qlonglong rowid);
    void donationEdit(qlonglong rowid);
    void donationNew();
    
    DbAdapter *db;
    
    QTableWidget *table = new QTableWidget;
    
signals:
    void signalImportDonations();
};




#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QCloseEvent>
#include "combo_currencies.h"
#include "people_selector.h"
class DonationsEdit : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsEdit(qlonglong rowid, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid;
    DbAdapter *db;
    QVBoxLayout *layout = new QVBoxLayout;
    qlonglong rowid_person;
    QString person_name;
    
    ComboCurrencies *combo_currencies;
    QPushButton *button_name = new QPushButton;
    
    QDateEdit *date = new QDateEdit;
    QLineEdit *edit_amount = new QLineEdit;
    QTextEdit *edit_memo = new QTextEdit;
    QLineEdit *edit_tnt_code = new QLineEdit;
    QLineEdit *edit_mandate_reference = new QLineEdit;
    QLineEdit *edit_bank_name = new QLineEdit;
    
    QDialog *dialog_select_person;
    void changePerson();
    void onSavePerson(qlonglong rowid, QString name);
    
signals:
    void signalUpdate();
    
public slots:
    void saveData();
    
};

#endif // DONATIONSLIST_H
