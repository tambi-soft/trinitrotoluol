#include "person_edit.h"

PersonEdit::PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
    , layout (new QGridLayout)
{
    setLayout(this->layout);
    
    this->rowid = rowid;
    this->db = db;
    
    drawGUI();
    loadData();
}

PersonEdit::PersonEdit(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QGridLayout)
{
    setLayout(this->layout);
    
    this->db = db;
    drawGUI();
}

void PersonEdit::drawGUI()
{
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
    this->layout->addWidget(edit_notes, 11, 1);
    
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
    this->layout->addWidget(new QLabel("Notes"), 11, 0);
    
    QPushButton *button_cancel = new QPushButton("cancel");
    this->layout->addWidget(button_cancel, 12, 0);
    QPushButton *button_save = new QPushButton("save");
    this->layout->addWidget(button_save, 12, 1);
    
    this->layout->setRowStretch(13, 100);
}

void PersonEdit::loadData()
{
    QMap<QString,QVariant> person = this->db->selectPerson(this->rowid);
    
    this->edit_tnt_id->setText(person["tnt_id"].toString());
    this->edit_name->setText(person["name"].toString());
    this->edit_email->setText(person["email"].toString());
    this->edit_address->setText(person["address"].toString());
    this->edit_phone->setText(person["phone"].toString());
    
    if (person["agreed_mail"].toInt() == 1)
    {
        this->check_agreed_mail->setChecked(true);
    }
    if (person["agreed_prayer"].toInt() == 1)
    {
        this->check_agreed_prayer->setChecked(true);
    }
    
    this->edit_agreement->setText(person["agreement"].toString());
    this->edit_donations_monthly->setText(person["donations_monthly"].toString());
    this->edit_donations_monthly_promised->setText(person["donations_monthly_promised"].toString());
    
    this->edit_spouse->setText(person["spouse_name"].toString());
}

void PersonEdit::save()
{
    /*
    this->db->insertNewPerson(this->edit_tnt_id->text(),
                              this->edit_name->text(),
                              this->edit_group,
                              this->edit_email->text(),
                              this->edit_address->text(),
                              this->edit_phone->text(),
                              this->check_agreed_mail->isChecked(),
                              this->check_agreed_prayer->isChecked(),
                              this->edit_agreement->text(),
                              this->edit_notes->toPlainText(),
                              this->edit_donations_monthly->text(),
                              this->edit_donations_monthly_promised->text());
                              */
}

void PersonEdit::clear()
{
    
}
