#ifndef PERSON_EDIT_H
#define PERSON_EDIT_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QResizeEvent>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QDate>
#include <QScrollArea>
#include <QSizePolicy>
#include <QMessageBox>

#include "db_adapter.h"
#include "person_groups.h"
#include "person_visits.h"
#include "person_relations.h"
#include "help_message.h"
#include "person_donations.h"

class PersonDetails : public QWidget
{
    Q_OBJECT
public:
    explicit PersonDetails(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    
private:
    QGroupBox *group_data;
    QGroupBox *group_visits;
    QGroupBox *group_mails;
    QGroupBox *group_donations;
    
private slots:
    void onDataChanged();
    void resizeEvent(QResizeEvent *event);
    
signals:
    void dataChanged();
};



class PersonEdit : public QWidget
{
    Q_OBJECT
public:
    explicit PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    
    void drawGUI();
    void loadData();
    /* pack all data in a qmap for beeing send to the db */
    QMap<QString,QVariant> collectSaveData();
    
private:
    QGridLayout *grid;
    qlonglong rowid = -1;
    DbAdapter *db;
    QList<QMap<QString,QVariant>> group_data;
    QMap<QString,int> group_data_map;
    
    QDialog *dialog_select_spouse;
    
    QCheckBox *check_todo = new QCheckBox;
    QCheckBox *check_waiting = new QCheckBox;
    
    QLineEdit *edit_tnt_id = new QLineEdit;
    QLineEdit *edit_name = new QLineEdit;
    QLineEdit *edit_email = new QLineEdit;
    QLineEdit *edit_address = new QLineEdit;
    QLineEdit *edit_phone = new QLineEdit;
    
    QComboBox *combo_group = new QComboBox;
    
    QCheckBox *check_agreed_mail = new QCheckBox;
    QCheckBox *check_agreed_prayer = new QCheckBox;
    
    QLineEdit *edit_agreement = new QLineEdit;
    QLineEdit *edit_donations_monthly = new QLineEdit;
    QLineEdit *edit_donations_monthly_promised = new QLineEdit;
    
    QLabel *edit_spouse = new QLabel;
    QTextEdit *edit_notes = new QTextEdit;
    
    PersonRelations *relations;
    PersonGroups *groups;
    
private slots:
    void onInsertAgreementDateButton();
    void saveDataWithInt(int /* param just for compat */);
    void saveData();
    
signals:
    void dataChanged();
};







class PersonMails : public GridWidget
{
    Q_OBJECT
public:
    explicit PersonMails(DbAdapter *db, qlonglong rowid, GridWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid_person;
    
    void showData();
};

#endif // PERSON_EDIT_H
