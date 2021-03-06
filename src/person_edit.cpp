#include "person_edit.h"

PersonDetails::PersonDetails(DbAdapter *db, qlonglong rowid, QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    //grid->setColumnStretch(0, 100000);
    //grid->setColumnStretch(1, 0);
    
    
    PersonEdit *edit = new PersonEdit(db, rowid);
    connect(edit, &PersonEdit::dataChanged, this, &PersonDetails::onDataChanged);
    PersonVisits *visits = new PersonVisits(db, rowid);
    PersonMails *mails = new PersonMails(db, rowid);
    PersonDonations *donations = new PersonDonations(db, rowid);
    
    this->group_data = new QGroupBox("Data");
    this->group_visits = new QGroupBox("Visits");
    this->group_mails = new QGroupBox("Mails");
    this->group_donations = new QGroupBox("Donations");
    
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

void PersonDetails::resizeEvent(QResizeEvent *event)
{
    int width = this->width();
    this->group_data->setMaximumWidth(width/2);
    this->group_visits->setMaximumWidth(width/2);
    this->group_mails->setMaximumWidth(width/2);
    this->group_donations->setMaximumWidth(width/2);
    
    QWidget::resizeEvent(event);
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
    
    QPushButton *button_tnt_help = new QPushButton;
    button_tnt_help->setIcon(QIcon::fromTheme("dialog-question"));
    button_tnt_help->setToolTip("help");
    connect(button_tnt_help, &QPushButton::clicked, this, []{ new HelpMessage(":help_tntware"); });
    
    this->grid->addWidget(edit_tnt_id, 2, 1, 1, 2);
    this->grid->addWidget(button_tnt_help, 2, 3);
    
    this->grid->addWidget(edit_name, 3, 1, 1, 3);
    this->grid->addWidget(edit_email, 4, 1, 1, 3);
    this->grid->addWidget(edit_address, 5, 1, 1, 3);
    this->grid->addWidget(edit_phone, 6, 1, 1, 3);
    
    this->grid->addWidget(check_agreed_mail, 8, 1);
    this->grid->addWidget(check_agreed_prayer, 9, 1);
    
    this->grid->addWidget(edit_agreement, 10, 1);
    QPushButton *button_agreement_today = new QPushButton("insert today");
    connect(button_agreement_today, &QPushButton::clicked, this, &PersonEdit::onInsertAgreementDateButton);
    QPushButton *button_agreement_help = new QPushButton;
    button_agreement_help->setIcon(QIcon::fromTheme("dialog-question"));
    button_agreement_help->setToolTip("help");
    connect(button_agreement_help, &QPushButton::clicked, this, []{ new HelpMessage(":help_agreed_date"); });
    this->grid->addWidget(button_agreement_today, 10, 2);
    this->grid->addWidget(button_agreement_help, 10, 3);
    
    this->grid->addWidget(edit_donations_monthly_promised, 12, 1, 1, 3);
    
    this->grid->addWidget(edit_notes, 14, 1, 1, 3);
    
    this->relations = new PersonRelations(this->db, this->rowid);
    this->grid->addWidget(this->relations, 15, 1, 1, 3);
    
    this->groups = new PersonGroups(this->db, this->rowid);
    this->grid->addWidget(this->groups, 16, 1, 1, 3);
    
    //this->grid->addWidget(new LineEditVCard(this->rowid, this->db, "ADR"), 17, 1, 1, 3);
    
    // =========================
    // 100 - 1000: Area for VCard-LineEdits
    // =========================
    
    this->grid->addWidget(this->combo_new_vcard_prop, 10000, 1, 1, 3); // row 10000 to keep this always at the bottom
    //connect(this->combo_new_vcard_prop, QOverload<int>::of(&ComboVCardProp::currentIndexChanged), &PersonEdit::onInsertVCardLineEdit);
    connect(this->combo_new_vcard_prop, &ComboVCardProp::vcardItemSelectedSignal, this, &PersonEdit::onNewVCardLineEdit);
    
    this->grid->addWidget(new QLabel("ToDo"), 0, 0);
    this->grid->addWidget(new QLabel("Waiting"), 1, 0);
    this->grid->addWidget(new QLabel("TntWare-Number"), 2, 0);
    this->grid->addWidget(new QLabel("Name"), 3, 0);
    this->grid->addWidget(new QLabel("Email"), 4, 0);
    this->grid->addWidget(new QLabel("Address"), 5, 0);
    this->grid->addWidget(new QLabel("Phone"), 6, 0);
    
    this->grid->addWidget(new QLabel("agreed mail"), 8, 0);
    this->grid->addWidget(new QLabel("agreed prayer"), 9, 0);
    this->grid->addWidget(new QLabel("agreement"), 10, 0);
    this->grid->addWidget(new QLabel("donations promised"), 12, 0);
    
    this->grid->addWidget(new QLabel("Notes"), 14, 0);
    this->grid->addWidget(new QLabel("Relations"), 15, 0);
    this->grid->addWidget(new QLabel("Groups"), 16, 0);
    
    this->grid->addWidget(new QLabel("Add New Property"), 10000, 0); // row 10000 to keep this always at the bottom
    
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
    
    
    QList<QMap<QString,QVariant>> data_vcf = this->db->personSelectVCardEntry(this->rowid);
    
    for (int i = 0; i < data_vcf.length(); i++)
    {
        onInsertVCardLineEdit(data_vcf.at(i)["rowid"].toLongLong(),
                data_vcf.at(i)["key"].toString(),
                data_vcf.at(i)["value"].toString());
    }
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
    data["donations_monthly_promised"] = this->edit_donations_monthly_promised->text().toInt();
    
    return data;
}

void PersonEdit::onInsertAgreementDateButton()
{
    QString current_date = QDate::currentDate().toString("yyyy-MM-dd");
    if (this->edit_agreement->text().length() <= 0)
    {
        this->edit_agreement->setText(current_date);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Overwrite current Value \""+this->edit_agreement->text()+"\" with \""+current_date+"\"?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes)
        {
            this->edit_agreement->setText(current_date);
        }
    }
}

void PersonEdit::onNewVCardLineEdit(QString vcard_item_name)
{
    qlonglong rowid_vcard = this->db->personNewVCardEntry(this->rowid, vcard_item_name);
    
    onInsertVCardLineEdit(rowid_vcard, vcard_item_name, "");
}
void PersonEdit::onInsertVCardLineEdit(qlonglong rowid_vcard, QString vcard_item_name, QString text)
{
    int line_pos = 100; // start inserting new LineEdits at grid line 100 (and up to line 1000)
    for (int i = 1000; i >= 100; --i) // we are counting backwards, because if an item is deleted, we want still insert the next one at the bottom
    {
        if (this->grid->itemAtPosition(i, 1) != nullptr)
        {
            line_pos = ++i;
            break;
        }
    }
    
    // select the label name (i.e. its position) in english
    ComboVCardProp *combo = new ComboVCardProp;
    QStringList list_standard = combo->properties_standard;
    int pos = 0;
    for (int i = 0; i < list_standard.length(); i++)
    {
        if (list_standard.at(i) == vcard_item_name)
        {
            pos = i;
        }
    }
    QStringList list_en = combo->properties_en;
    QLabel *label = new QLabel(list_en.at(pos));
    this->grid->addWidget(label, line_pos, 0);
    
    LineEditVCard *vcard_edit = new LineEditVCard(this->rowid, this->db, rowid_vcard, vcard_item_name);
    vcard_edit->setText(text);
    this->grid->addWidget(vcard_edit, line_pos, 1, 1, 2);
    
    QPushButton *button_delete_vcard = new QPushButton;
    button_delete_vcard->setIcon(QIcon::fromTheme("edit-delete"));
    button_delete_vcard->setToolTip("delete this entry");
    connect(button_delete_vcard, &QPushButton::clicked, this, [this, rowid_vcard, label, vcard_edit, button_delete_vcard]{ onDeleteVCardLineEdit(rowid_vcard, label, vcard_edit, button_delete_vcard); });
    this->grid->addWidget(button_delete_vcard, line_pos, 3);
}
void PersonEdit::onDeleteVCardLineEdit(qlonglong rowid_vcard, QLabel *label, LineEditVCard *vcard, QPushButton *button)
{
    this->db->personDeleteVCardEntry(rowid_vcard);
    
    this->grid->removeWidget(label);
    this->grid->removeWidget(vcard);
    this->grid->removeWidget(button);
    
    label->deleteLater();
    vcard->deleteLater();
    button->deleteLater();
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
