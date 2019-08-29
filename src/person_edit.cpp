#include "person_edit.h"

PersonEdit::PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
    , layout (new QGridLayout)
{
    setLayout(this->layout);
    
    this->rowid = rowid;
    this->db = db;
    populateGUI();
}

void PersonEdit::drawGUI()
{
    
}

void PersonEdit::populateGUI()
{
    QMap<QString,QVariant> person = this->db->selectPerson(this->rowid);
    qDebug() << person << this->rowid;
    
    QLineEdit *edit_tnt_id = new QLineEdit();
    QLineEdit *edit_name = new QLineEdit();
    QLineEdit *edit_email = new QLineEdit();
    QLineEdit *edit_address = new QLineEdit();
    QLineEdit *edit_phone = new QLineEdit();
    
    QCheckBox *check_agreed_mail = new QCheckBox();
    QCheckBox *check_agreed_prayer = new QCheckBox();
    
    QLineEdit *edit_agreement = new QLineEdit();
    QLineEdit *edit_donations_monthly = new QLineEdit();
    QLineEdit *edit_donations_monthly_promised = new QLineEdit();
    
    QLabel *edit_spouse = new QLabel();
    
    edit_tnt_id->setText(person["tnt_id"].toString());
    edit_name->setText(person["name"].toString());
    edit_email->setText(person["email"].toString());
    edit_address->setText(person["address"].toString());
    edit_phone->setText(person["phone"].toString());
    
    if (person["agreed_mail"].toInt() == 1)
    {
        check_agreed_mail->setChecked(true);
    }
    if (person["agreed_prayer"].toInt() == 1)
    {
        check_agreed_prayer->setChecked(true);
    }
    
    edit_agreement->setText(person["agreement"].toString());
    edit_donations_monthly->setText(person["donations_monthly"].toString());
    edit_donations_monthly_promised->setText(person["donations_monthly_promised"].toString());
    
    edit_spouse->setText(person["spouse_name"].toString());
    
    this->layout->addWidget(edit_tnt_id, 0, 1);
    this->layout->addWidget(edit_name, 1, 1);
    this->layout->addWidget(edit_email, 2, 1);
    this->layout->addWidget(edit_address, 3, 1);
    this->layout->addWidget(edit_phone, 4, 1);
    
    this->layout->addWidget(check_agreed_mail, 5, 1);
    this->layout->addWidget(check_agreed_prayer, 6, 1);
    
    this->layout->addWidget(edit_agreement, 7, 1);
    this->layout->addWidget(edit_donations_monthly, 8, 1);
    this->layout->addWidget(edit_donations_monthly_promised, 9, 1);
    this->layout->addWidget(edit_spouse, 10, 1);
    
    this->layout->addWidget(new QLabel("TNT-Number"), 0, 0);
    this->layout->addWidget(new QLabel("Name"), 1, 0);
    this->layout->addWidget(new QLabel("Email"), 2, 0);
    this->layout->addWidget(new QLabel("Address"), 3, 0);
    this->layout->addWidget(new QLabel("Phone"), 4, 0);
    this->layout->addWidget(new QLabel("agreed mail"), 5, 0);
    this->layout->addWidget(new QLabel("agreed prayer"), 6, 0);
    this->layout->addWidget(new QLabel("agreement"), 7, 0);
    this->layout->addWidget(new QLabel("donations monthly"), 8, 0);
    this->layout->addWidget(new QLabel("donations promised"), 9, 0);
    this->layout->addWidget(new QLabel("Spouse"), 10, 0);
    
    this->layout->setRowStretch(11, 100);
}

void PersonEdit::clear()
{
    
}
