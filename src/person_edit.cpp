#include "person_edit.h"

PersonDetails::PersonDetails(DbAdapter *db, qlonglong rowid, QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    PersonEdit *edit = new PersonEdit(db, rowid);
    connect(edit, &PersonEdit::dataChanged, this, &PersonDetails::onDataChanged);
    PersonVisits *visits = new PersonVisits(db, rowid);
    PersonMails *mails = new PersonMails(db, rowid);
    
    QGroupBox *group_data = new QGroupBox("Data");
    QGroupBox *group_visits = new QGroupBox("Visits");
    QGroupBox *group_mails = new QGroupBox("Mails");
    
    QVBoxLayout *layout_data = new QVBoxLayout;
    QVBoxLayout *layout_visits = new QVBoxLayout;
    QVBoxLayout *layout_mails = new QVBoxLayout;
    
    group_data->setLayout(layout_data);
    group_visits->setLayout(layout_visits);
    group_mails->setLayout(layout_mails);
    
    layout_data->addWidget(edit);
    layout_visits->addWidget(visits);
    layout_mails->addWidget(mails);
    
    //grid->addWidget(edit, 0, 0, 2, 1);
    grid->addWidget(group_data, 0, 0, 2, 1);
    grid->addWidget(group_visits, 0, 1);
    grid->addWidget(group_mails, 1, 1);
}

void PersonDetails::onDataChanged()
{
    emit dataChanged();
}



PersonEdit::PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
{
    this->grid = new QGridLayout;
    setLayout(this->grid);
    this->grid->setMargin(0);
    
    this->rowid = rowid;
    this->db = db;
    
    drawGUI();
}

void PersonEdit::drawGUI()
{
    this->grid->addWidget(check_todo, 0, 1);
    this->grid->addWidget(check_waiting, 1, 1);
    
    this->grid->addWidget(edit_tnt_id, 2, 1, 1, 2);
    this->grid->addWidget(edit_name, 3, 1, 1, 2);
    this->grid->addWidget(edit_email, 4, 1, 1, 2);
    this->grid->addWidget(edit_address, 5, 1, 1, 2);
    this->grid->addWidget(edit_phone, 6, 1, 1, 2);
    
    QPushButton *button_edit_groups = new QPushButton("Edit Groups");
    connect(button_edit_groups, &QPushButton::clicked, this, &PersonEdit::onAddNewGroupButton);
    this->grid->addWidget(combo_group, 7, 1);
    this->grid->addWidget(button_edit_groups, 7, 2);
    
    this->grid->addWidget(check_agreed_mail, 8, 1);
    this->grid->addWidget(check_agreed_prayer, 9, 1);
    
    this->grid->addWidget(edit_agreement, 10, 1);
    QPushButton *button_agreement_today = new QPushButton("insert today");
    connect(button_agreement_today, &QPushButton::clicked, this, &PersonEdit::onInsertAgreementDateButton);
    this->grid->addWidget(button_agreement_today, 10, 2);
    this->edit_agreement->setToolTip("probably you want to store the date of the according email from this supporter, or the name of a scanned file, or some other hint to find the actual agreement easily");
    
    this->grid->addWidget(edit_donations_monthly, 11, 1, 1, 2);
    this->grid->addWidget(edit_donations_monthly_promised, 12, 1, 1, 2);
    
    QPushButton *button_select_spouse = new QPushButton("Select Spouse");
    connect(button_select_spouse, &QPushButton::clicked, this, &PersonEdit::onSelectSpouseButton);
    this->grid->addWidget(edit_spouse, 13, 1);
    this->grid->addWidget(button_select_spouse, 13, 2);
    
    this->grid->addWidget(edit_notes, 14, 1, 1, 2);
    
    this->grid->addWidget(new QLabel("ToDo"), 0, 0);
    this->grid->addWidget(new QLabel("Waiting"), 1, 0);
    this->grid->addWidget(new QLabel("TNT-Number"), 2, 0);
    this->grid->addWidget(new QLabel("Name"), 3, 0);
    this->grid->addWidget(new QLabel("Email"), 4, 0);
    this->grid->addWidget(new QLabel("Address"), 5, 0);
    this->grid->addWidget(new QLabel("Phone"), 6, 0);
    this->grid->addWidget(new QLabel("Group"), 7, 0);
    this->grid->addWidget(new QLabel("agreed mail"), 8, 0);
    this->grid->addWidget(new QLabel("agreed prayer"), 9, 0);
    this->grid->addWidget(new QLabel("agreement"), 10, 0);
    this->grid->addWidget(new QLabel("donations monthly"), 11, 0);
    this->grid->addWidget(new QLabel("donations promised"), 12, 0);
    this->grid->addWidget(new QLabel("Spouse"), 13, 0);
    this->grid->addWidget(new QLabel("Notes"), 14, 0);
    
    loadGroupsComboData();
    loadData();
    
    QList<QCheckBox*> allCheckboxesToAutosave;
    allCheckboxesToAutosave << check_todo << check_waiting << check_agreed_mail << check_agreed_prayer;
    QList<QLineEdit*> allLineEditsToAutosave;
    allLineEditsToAutosave << edit_tnt_id << edit_name << edit_email << edit_address << edit_phone << edit_agreement << edit_donations_monthly << edit_donations_monthly_promised;
    foreach(QCheckBox *item, allCheckboxesToAutosave)
    {
        connect(item, &QCheckBox::stateChanged, this, &PersonEdit::saveData);
    }
    foreach(QLineEdit *item, allLineEditsToAutosave)
    {
        connect(item, &QLineEdit::textChanged, this, &PersonEdit::saveData);
    }
    
    connect(combo_group, qOverload<int>(&QComboBox::currentIndexChanged), this, &PersonEdit::saveDataWithInt);
    connect(edit_notes, &QTextEdit::textChanged, this, &PersonEdit::saveData);
                               
    //this->grid->setRowStretch(14, 100);
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

void PersonEdit::onAddNewGroupButton()
{
    
}

void PersonEdit::onInsertAgreementDateButton()
{
    this->edit_agreement->setText(QDate::currentDate().toString("yyyy-MM-dd"));
}

void PersonEdit::onSelectSpouseButton()
{
    PeopleSelector *selector = new PeopleSelector(this->db);
    connect(selector, &PeopleSelector::personSelected, this, &PersonEdit::onSpouseSelected);
    
    this->dialog_select_spouse = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    this->dialog_select_spouse->setLayout(layout_dialog);
    layout_dialog->addWidget(selector);
    
    this->dialog_select_spouse->exec();
}

void PersonEdit::onSpouseSelected(qlonglong rowid, QString name)
{
    this->dialog_select_spouse->close();
    
    this->db->linkSpouses(this->rowid, rowid);
    
    this->edit_spouse->setText(name);
}

void PersonEdit::saveDataWithInt(int /*param just for compat*/)
{
    saveData();
}
void PersonEdit::saveData()
{
    QMap<QString,QVariant> data = collectSaveData();
    this->db->updatePerson(this->rowid, data);
    
    emit dataChanged();
}



PersonVisits::PersonVisits(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid;
    setLayout(this->layout);
    this->layout->setMargin(0);
    
    this->layout->addWidget(this->table);
    
    showData();
}

void PersonVisits::showData()
{
    QList<QMap<QString,QVariant>> data = this->db->selectVisitsForPerson(this->rowid_person);
    
    QStringList headers;
    headers << "journey_name" << "date" << "notes";
    
    this->table->setColumnCount(headers.length());
    this->table->setRowCount(data.length());
    this->table->setHorizontalHeaderLabels(headers);
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> visit = data.at(i);
        
        this->table->setItem(i, 0, new QTableWidgetItem(visit["journey_name"].toString()));
        this->table->setItem(i, 1, new QTableWidgetItem(visit["date"].toString()));
        this->table->setItem(i, 2, new QTableWidgetItem(visit["notes"].toString()));
    }
    
    //this->table->resizeRowsToContents();
    this->table->resizeColumnsToContents();
}



PersonMails::PersonMails(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid;
    setLayout(this->layout);
    this->layout->setMargin(0);
    
    this->layout->addWidget(this->table);
    
    showData();
}

void PersonMails::showData()
{
    QList<QMap<QString,QVariant>> data = this->db->selectMailsForPerson(this->rowid_person);
    
    QStringList headers;
    headers << "number" << "date";
    
    this->table->setColumnCount(headers.length());
    this->table->setRowCount(data.length());
    this->table->setHorizontalHeaderLabels(headers);
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> mail = data.at(i);
        
        this->table->setItem(i, 0, new QTableWidgetItem(mail["number"].toString()));
        this->table->setItem(i, 1, new QTableWidgetItem(mail["date"].toString()));
    }
    
    //this->table->resizeRowsToContents();
    this->table->resizeColumnsToContents();
}
