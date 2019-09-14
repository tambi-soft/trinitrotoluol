#include "person_edit.h"

PersonEdit::PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
{
    this->layout = new QGridLayout;
    setLayout(this->layout);
    
    this->rowid = rowid;
    this->db = db;
    
    drawGUI();
    loadData();
}

PersonEdit::PersonEdit(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
{
    this->layout = new QGridLayout;
    setLayout(this->layout);
    
    this->db = db;
    drawGUI();
}

void PersonEdit::drawGUI()
{
    this->layout->addWidget(check_todo, 0, 1);
    this->layout->addWidget(check_waiting, 1, 1);
    
    this->layout->addWidget(edit_tnt_id, 2, 1);
    this->layout->addWidget(edit_name, 3, 1);
    this->layout->addWidget(edit_email, 4, 1);
    this->layout->addWidget(edit_address, 5, 1);
    this->layout->addWidget(edit_phone, 6, 1);
    
    QWidget *widget_groups = new QWidget;
    QHBoxLayout *layout_groups = new QHBoxLayout;
    layout_groups->setMargin(0);
    QPushButton *button_edit_groups = new QPushButton("Edit Groups");
    connect(button_edit_groups, &QPushButton::clicked, this, &PersonEdit::onAddNewGroupButton);
    layout_groups->addWidget(combo_group);
    layout_groups->addWidget(button_edit_groups);
    widget_groups->setLayout(layout_groups);
    //this->layout->addWidget(combo_group, 5, 1);
    this->layout->addWidget(widget_groups, 7, 1);
    
    this->layout->addWidget(check_agreed_mail, 8, 1);
    this->layout->addWidget(check_agreed_prayer, 9, 1);
    
    this->layout->addWidget(edit_agreement, 10, 1);
    this->layout->addWidget(edit_donations_monthly, 11, 1);
    this->layout->addWidget(edit_donations_monthly_promised, 12, 1);
    
    QWidget *widget_spouse = new QWidget;
    QHBoxLayout *layout_spouse = new QHBoxLayout;
    layout_spouse->setMargin(0);
    QPushButton *button_select_spouse = new QPushButton("Select Spouse");
    connect(button_select_spouse, &QPushButton::clicked, this, &PersonEdit::onSelectSpouseButton);
    layout_spouse->addWidget(edit_spouse);
    layout_spouse->addWidget(button_select_spouse);
    widget_spouse->setLayout(layout_spouse);
    this->layout->addWidget(widget_spouse, 13, 1);
    //this->layout->addWidget(edit_spouse, 11, 1);
    
    this->layout->addWidget(edit_notes, 14, 1);
    
    this->layout->addWidget(new QLabel("ToDo"), 0, 0);
    this->layout->addWidget(new QLabel("Waiting"), 1, 0);
    this->layout->addWidget(new QLabel("TNT-Number"), 2, 0);
    this->layout->addWidget(new QLabel("Name"), 3, 0);
    this->layout->addWidget(new QLabel("Email"), 4, 0);
    this->layout->addWidget(new QLabel("Address"), 5, 0);
    this->layout->addWidget(new QLabel("Phone"), 6, 0);
    this->layout->addWidget(new QLabel("Group"), 7, 0);
    this->layout->addWidget(new QLabel("agreed mail"), 8, 0);
    this->layout->addWidget(new QLabel("agreed prayer"), 9, 0);
    this->layout->addWidget(new QLabel("agreement"), 10, 0);
    this->layout->addWidget(new QLabel("donations monthly"), 11, 0);
    this->layout->addWidget(new QLabel("donations promised"), 12, 0);
    this->layout->addWidget(new QLabel("Spouse"), 13, 0);
    this->layout->addWidget(new QLabel("Notes"), 14, 0);
    
    QPushButton *button_cancel = new QPushButton("cancel");
    this->layout->addWidget(button_cancel, 15, 0);
    QPushButton *button_save = new QPushButton("save");
    this->layout->addWidget(button_save, 15, 1);
    
    connect(button_cancel, &QPushButton::clicked, this, &PersonEdit::onCancelButton);
    connect(button_save, &QPushButton::clicked, this, &PersonEdit::onSaveButton);
    //this->layout->setRowStretch(14, 100);
    
    loadGroupsComboData();
}

void PersonEdit::loadData()
{
    QMap<QString,QVariant> person = this->db->selectPerson(this->rowid);
    
    if (person["flag_todo"].toInt() == 1)
    {
        this->check_todo->setChecked(true);
    }
    if (person["flag_waiting"].toInt() == 1)
    {
        this->check_waiting->setChecked(true);
    }
    
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

QMap<QString,QVariant> PersonEdit::collectSaveData()
{
    QString group_str = this->combo_group->currentText();
    int group = this->group_data_map[group_str];
    
    QMap<QString,QVariant> data;
    data["flag_todo"] = this->check_todo->isChecked();
    data["flag_waiting"] = this->check_waiting->isChecked();
    data["tnt_id"] = this->edit_tnt_id->text();
    data["name"] = this->edit_name->text();
    data["group"] = group;
    data["email"] = this->edit_email->text();
    data["address"] = this->edit_address->text();
    data["phone"] = this->edit_phone->text();
    data["agreed_mail"] = this->check_agreed_mail->isChecked();
    data["agreed_prayer"] = this->check_agreed_prayer->isChecked();
    data["agreement"] = this->edit_agreement->text();
    data["notes"] = this->edit_notes->toPlainText();
    data["donations_monthly"] = this->edit_donations_monthly->text().toInt();
    data["donations_monthly_promised"] = this->edit_donations_monthly_promised->text().toInt();
    
    return data;
}

void PersonEdit::savePerson()
{
    QMap<QString,QVariant> data = collectSaveData();
    
    this->rowid = this->db->insertNewPerson(data);
    qDebug() << this->rowid;
}

void PersonEdit::updatePerson()
{
    QMap<QString,QVariant> data = collectSaveData();
    
    this->db->updatePerson(this->rowid, data);
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
        
        // convert this view from an "adding a new person" to "editing an existing one"
        delete this->layout;
        PersonEdit(this->db, this->rowid);
    }
    else
    {
        updatePerson();
    }
}
