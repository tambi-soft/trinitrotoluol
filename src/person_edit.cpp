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
    
    QWidget *widget_groups = new QWidget;
    QHBoxLayout *layout_groups = new QHBoxLayout;
    layout_groups->setMargin(0);
    QPushButton *button_edit_groups = new QPushButton("Edit Groups");
    connect(button_edit_groups, &QPushButton::clicked, this, &PersonEdit::onAddNewGroupButton);
    layout_groups->addWidget(combo_group);
    layout_groups->addWidget(button_edit_groups);
    widget_groups->setLayout(layout_groups);
    //this->layout->addWidget(combo_group, 5, 1);
    this->layout->addWidget(widget_groups, 5, 1);
    
    this->layout->addWidget(check_agreed_mail, 6, 1);
    this->layout->addWidget(check_agreed_prayer, 7, 1);
    
    this->layout->addWidget(edit_agreement, 8, 1);
    this->layout->addWidget(edit_donations_monthly, 9, 1);
    this->layout->addWidget(edit_donations_monthly_promised, 10, 1);
    
    QWidget *widget_spouse = new QWidget;
    QHBoxLayout *layout_spouse = new QHBoxLayout;
    layout_spouse->setMargin(0);
    QPushButton *button_select_spouse = new QPushButton("Select Spouse");
    connect(button_select_spouse, &QPushButton::clicked, this, &PersonEdit::onSelectSpouseButton);
    layout_spouse->addWidget(edit_spouse);
    layout_spouse->addWidget(button_select_spouse);
    widget_spouse->setLayout(layout_spouse);
    this->layout->addWidget(widget_spouse, 11, 1);
    //this->layout->addWidget(edit_spouse, 11, 1);
    
    this->layout->addWidget(edit_notes, 12, 1);
    
    this->layout->addWidget(new QLabel("TNT-Number"), 0, 0);
    this->layout->addWidget(new QLabel("Name"), 1, 0);
    this->layout->addWidget(new QLabel("Email"), 2, 0);
    this->layout->addWidget(new QLabel("Address"), 3, 0);
    this->layout->addWidget(new QLabel("Phone"), 4, 0);
    this->layout->addWidget(new QLabel("Group"), 5, 0);
    this->layout->addWidget(new QLabel("agreed mail"), 6, 0);
    this->layout->addWidget(new QLabel("agreed prayer"), 7, 0);
    this->layout->addWidget(new QLabel("agreement"), 8, 0);
    this->layout->addWidget(new QLabel("donations monthly"), 9, 0);
    this->layout->addWidget(new QLabel("donations promised"), 10, 0);
    this->layout->addWidget(new QLabel("Spouse"), 11, 0);
    this->layout->addWidget(new QLabel("Notes"), 12, 0);
    
    QPushButton *button_cancel = new QPushButton("cancel");
    this->layout->addWidget(button_cancel, 13, 0);
    QPushButton *button_save = new QPushButton("save");
    this->layout->addWidget(button_save, 13, 1);
    
    connect(button_cancel, &QPushButton::clicked, this, &PersonEdit::onCancelButton);
    connect(button_save, &QPushButton::clicked, this, &PersonEdit::onSaveButton);
    //this->layout->setRowStretch(14, 100);
    
    loadGroupsComboData();
}

void PersonEdit::loadData()
{
    QMap<QString,QVariant> person = this->db->selectPerson(this->rowid);
    
    this->edit_tnt_id->setText(person["tnt_id"].toString());
    this->edit_name->setText(person["name"].toString());
    this->edit_email->setText(person["email"].toString());
    this->edit_address->setText(person["address"].toString());
    this->edit_phone->setText(person["phone"].toString());
    
    this->combo_group->setCurrentText(person["group_name"].toString());
    
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
    
    this->edit_notes->setText(person["notes"].toString());
}

void PersonEdit::loadGroupsComboData()
{
    this->group_data = this->db->selectGroups();
    
    QList<QString> gr;
    for (int i=0; i < group_data.length(); ++i)
    {
        gr.append(group_data.at(i)["name"].toString());
        
        int rowid = group_data.at(i)["rowid"].toInt();
        QString group = group_data.at(i)["name"].toString();
        this->group_data_map[group] = rowid;
    }
    
    this->combo_group->addItems(gr);
}

void PersonEdit::savePerson()
{
    QString group_str = this->combo_group->currentText();
    int group = this->group_data_map[group_str];
    
    this->db->insertNewPerson(this->edit_tnt_id->text(),
                              this->edit_name->text(),
                              group,
                              this->edit_email->text(),
                              this->edit_address->text(),
                              this->edit_phone->text(),
                              this->check_agreed_mail->isChecked(),
                              this->check_agreed_prayer->isChecked(),
                              this->edit_agreement->text(),
                              this->edit_notes->toPlainText(),
                              this->edit_donations_monthly->text().toInt(),
                              this->edit_donations_monthly_promised->text().toInt()
                          );
}

void PersonEdit::updatePerson()
{
    
}

void PersonEdit::clear()
{
    
}

void PersonEdit::onAddNewGroupButton()
{
    
}

void PersonEdit::onSelectSpouseButton()
{
    
}

void PersonEdit::onCancelButton()
{
    emit closeCurrentTabSignal();
}

void PersonEdit::onSaveButton()
{
    if (this->rowid == -1)
    {
        savePerson();
    }
    else
    {
        updatePerson();
    }
}
