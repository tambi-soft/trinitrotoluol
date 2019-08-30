#ifndef PERSON_EDIT_H
#define PERSON_EDIT_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>

#include "db_adapter.h"

class PersonEdit : public QWidget
{
    Q_OBJECT
public:
    explicit PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    explicit PersonEdit(DbAdapter *db, QWidget *parent = nullptr);
    
    void drawGUI();
    void populateGUI();
    void clear();
private:
    QGridLayout *layout;
    qlonglong rowid;
    DbAdapter *db;
    
    QLineEdit *edit_tnt_id;
    QLineEdit *edit_name;
    QLineEdit *edit_email;
    QLineEdit *edit_address;
    QLineEdit *edit_phone;
    
    QCheckBox *check_agreed_mail;
    QCheckBox *check_agreed_prayer;
    
    QLineEdit *edit_agreement;
    QLineEdit *edit_donations_monthly;
    QLineEdit *edit_donations_monthly_promised;
    
    QLabel *edit_spouse = new QLabel();
    
};

#endif // PERSON_EDIT_H
