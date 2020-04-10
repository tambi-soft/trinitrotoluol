#include "person_edit.h"

PersonDetails::PersonDetails(DbAdapter *db, qlonglong rowid, QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    PersonEdit *edit = new PersonEdit(db, rowid);
    connect(edit, &PersonEdit::dataChanged, this, &PersonDetails::onDataChanged);
    PersonVisits *visits = new PersonVisits(db, rowid);
    PersonMails *mails = new PersonMails(db, rowid);
    PersonDonations *donations = new PersonDonations(db, rowid);
    
    QGroupBox *group_data = new QGroupBox("Data");
    QGroupBox *group_visits = new QGroupBox("Visits");
    QGroupBox *group_mails = new QGroupBox("Mails");
    QGroupBox *group_donations = new QGroupBox("Donations");
    
    QVBoxLayout *layout_data = new QVBoxLayout;
    QVBoxLayout *layout_visits = new QVBoxLayout;
    QVBoxLayout *layout_mails = new QVBoxLayout;
    QVBoxLayout *layout_donations = new QVBoxLayout;
    
    group_data->setLayout(layout_data);
    group_visits->setLayout(layout_visits);
    group_mails->setLayout(layout_mails);
    group_donations->setLayout(layout_donations);
    
    QScrollArea *scroll_data = new QScrollArea(this);
    scroll_data->setWidgetResizable(true);
    layout_data->addWidget(scroll_data);
    QVBoxLayout *layout_data_scroll = new QVBoxLayout;
    QWidget *scroll_widget = new QWidget;
    //scroll_data->setLayout(layout_data_scroll);
    scroll_data->setWidget(scroll_widget);
    scroll_widget->setLayout(layout_data_scroll);
    layout_data_scroll->addWidget(edit);
    
    //layout_data->addWidget(edit);
    layout_visits->addWidget(visits);
    layout_mails->addWidget(mails);
    layout_donations->addWidget(donations);
    
    grid->addWidget(group_data, 0, 0, 3, 1);
    grid->addWidget(group_visits, 0, 1);
    grid->addWidget(group_mails, 1, 1);
    grid->addWidget(group_donations, 2, 1);
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
    
    setMinimumHeight(740);
    
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
    
    this->grid->addWidget(check_agreed_mail, 8, 1);
    this->grid->addWidget(check_agreed_prayer, 9, 1);
    
    this->grid->addWidget(edit_agreement, 10, 1);
    QPushButton *button_agreement_today = new QPushButton("insert today");
    connect(button_agreement_today, &QPushButton::clicked, this, &PersonEdit::onInsertAgreementDateButton);
    this->grid->addWidget(button_agreement_today, 10, 2);
    this->edit_agreement->setToolTip("probably you want to store the date of the according email from this supporter, or the name of a scanned file, or some other hint to find the actual agreement easily");
    
    this->grid->addWidget(edit_donations_monthly, 11, 1, 1, 2);
    this->grid->addWidget(edit_donations_monthly_promised, 12, 1, 1, 2);
    
    //QPushButton *button_select_spouse = new QPushButton("Select Spouse");
    //connect(button_select_spouse, &QPushButton::clicked, this, &PersonEdit::onSelectSpouseButton);
    //this->grid->addWidget(edit_spouse, 13, 1);
    //this->grid->addWidget(button_select_spouse, 13, 2);
    
    this->grid->addWidget(edit_notes, 14, 1, 1, 2);
    
    this->relations = new PersonRelations(this->db, this->rowid);
    this->grid->addWidget(this->relations, 15, 1, 1, 2);
    
    this->groups = new PersonGroups(this->db, this->rowid);
    this->grid->addWidget(this->groups, 16, 1, 1, 2);
    
    this->grid->addWidget(new QLabel("ToDo"), 0, 0);
    this->grid->addWidget(new QLabel("Waiting"), 1, 0);
    this->grid->addWidget(new QLabel("TNT-Number"), 2, 0);
    this->grid->addWidget(new QLabel("Name"), 3, 0);
    this->grid->addWidget(new QLabel("Email"), 4, 0);
    this->grid->addWidget(new QLabel("Address"), 5, 0);
    this->grid->addWidget(new QLabel("Phone"), 6, 0);
    
    this->grid->addWidget(new QLabel("agreed mail"), 8, 0);
    this->grid->addWidget(new QLabel("agreed prayer"), 9, 0);
    this->grid->addWidget(new QLabel("agreement"), 10, 0);
    this->grid->addWidget(new QLabel("donations monthly"), 11, 0);
    this->grid->addWidget(new QLabel("donations promised"), 12, 0);
    this->grid->addWidget(new QLabel("Spouse"), 13, 0);
    this->grid->addWidget(new QLabel("Notes"), 14, 0);
    this->grid->addWidget(new QLabel("Relations"), 15, 0);
    this->grid->addWidget(new QLabel("Groups"), 16, 0);
    
    //loadGroupsComboData();
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
    
    //connect(combo_group, qOverload<int>(&QComboBox::currentIndexChanged), this, &PersonEdit::saveDataWithInt);
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

/*
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
*/

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







PersonMails::PersonMails(DbAdapter *db, qlonglong rowid, GridWidget *parent)
    : GridWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid;
    
    showData();
}

void PersonMails::showData()
{
    recreateView();
    
    QList<QMap<QString,QVariant>> data = this->db->selectMailsForPerson(this->rowid_person);
    
    this->grid->addWidget(new QLabel("<b>Number</b>"), 0, 0);
    this->grid->addWidget(new QLabel("<b>Date</b>"), 0, 1);
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> mail = data.at(i);
        
        this->grid->addWidget(new QLabel(mail["number"].toString()), i+1, 0);
        this->grid->addWidget(new QLabel(mail["date"].toString()), i+1, 1);
    }
}



PersonDonations::PersonDonations(DbAdapter *db, qlonglong rowid, GridWidget *parent)
    : GridWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid;
    
    showData();
}

void PersonDonations::showData()
{
    recreateView();
    
    QList<QMap<QString,QVariant>> data = this->db->donationsSelectForPerson(this->rowid_person);
    
    this->grid->addWidget(new QLabel("<b>Amount</b>"), 0, 0);
    this->grid->addWidget(new QLabel("<b>Currency</b>"), 0, 1);
    this->grid->addWidget(new QLabel("<b>Date</b>"), 0, 2);
    this->grid->addWidget(new QLabel("<b>Memo</b>"), 0, 3);
    
    QMap<QString,QVariant> currency_default = this->db->currencySelectDefault();
    double sum = 0;
    for (int i=0; i < data.length(); ++i)
    {
        sum += data.at(i)["amount"].toDouble() * data.at(i)["exchange_rate"].toDouble();
        
        QLabel *label_amount = new QLabel(data.at(i)["amount"].toString());
        QLabel *label_currency = new QLabel(data.at(i)["code"].toString());
        QLabel *label_date = new QLabel(data.at(i)["date"].toString());
        QLabel *label_memo = new QLabel(data.at(i)["memo"].toString());
        
        this->grid->addWidget(label_amount, i+1, 0);
        this->grid->addWidget(label_currency, i+1, 1);
        this->grid->addWidget(label_date, i+1, 2);
        this->grid->addWidget(label_memo, i+1, 3);
    }
    
    QLabel *label_sum_name = new QLabel("<b>Sum:</b>");
    QLabel *label_sum_value = new QLabel(QString::number(sum));
    QLabel *label_sum_currency = new QLabel(currency_default["code"].toString());
    
    // spacer
    this->grid->addWidget(new QLabel(""), data.length()+9, 0);
    
    this->grid->addWidget(label_sum_name, data.length()+10, 0);
    this->grid->addWidget(label_sum_value, data.length()+10, 1);
    this->grid->addWidget(label_sum_currency, data.length()+10, 2);
}
