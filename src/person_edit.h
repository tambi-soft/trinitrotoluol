#ifndef PERSON_EDIT_H
#define PERSON_EDIT_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include "db_adapter.h"

class PersonEdit : public QWidget
{
    Q_OBJECT
public:
    explicit PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    explicit PersonEdit(DbAdapter *db, QWidget *parent = nullptr);
    
    void drawGUI();
    void loadData();
    void save();
    void clear();
    
private:
    QGridLayout *layout;
    qlonglong rowid = -1;
    DbAdapter *db;
    
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
};

#endif // PERSON_EDIT_H
