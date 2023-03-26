#include "donations_list.h"

DonationsList::DonationsList(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->addWidget(this->table);
    
    //QPushButton *button_import = new QPushButton("import donations");
    //connect(button_import, &QPushButton::clicked, this, &DonationsList::signalImportDonations);
    //layout->addWidget(button_import);
    
    QPushButton *button_new = new QPushButton("new entry");
    connect(button_new, &QPushButton::clicked, this, &DonationsList::donationNew);
    layout->addWidget(button_new);
}

void DonationsList::showData()
{
    this->table->clear();
    
    QList<QMap<QString,QVariant>> data_monthly = this->db->donationsByMonth();
    QMap<QString,QVariant> data_monthly_map;
    for (int i=0; i < data_monthly.length(); i++)
    {
        data_monthly_map[data_monthly.at(i)["year-month"].toString()] = data_monthly.at(i)["amount"].toString();
    }
    
    QList<QMap<QString,QVariant>> data = this->db->donationsSelect();
    
    this->table->setColumnCount(6);
    int row_count = data.length();
    this->table->setRowCount(row_count);
    
    this->table->horizontalHeader()->setVisible(false);
    this->table->verticalHeader()->setVisible(false);
    this->table->setStyleSheet(QString("QTableView { gridline-color: rgba(255,0,0,0.0); }"));
    
    QDate date_last = QDate::currentDate();
    int y = 0;
    for (int i=0; i < data.length(); ++i)
    {
        ++y;
        
        QMap<QString,QVariant> donation = data.at(i);
        
        QDate date_current = QDate::fromString(donation["date"].toString(), "yyyy-MM-dd");
        
        if ((date_last.month() != date_current.month()) && i >= 2)
        {
            QString sum = data_monthly_map[date_last.toString("yyyy-MM")].toString();
            this->table->setCellWidget(y, 3, new QLabel("<b>Σ: "+sum+"</b>"));
            y++;
            row_count++;
            
            y++;
            row_count++;
            this->table->setRowCount(row_count);
        }
        
        date_last = date_current;
        
        QTableWidgetItem *item_name = new QTableWidgetItem(donation["name"].toString());
        item_name->setFlags(item_name->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *item_amount = new QTableWidgetItem(donation["amount"].toString());
        item_amount->setFlags(item_amount->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *item_code = new QTableWidgetItem(donation["code"].toString());
        item_code->setFlags(item_code->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *item_date = new QTableWidgetItem(donation["date"].toString());
        item_date->setFlags(item_date->flags() ^ Qt::ItemIsEditable);
        
        this->table->setItem(y, 2, item_name);
        this->table->setItem(y, 3, item_amount);
        this->table->setItem(y, 4, item_code);
        this->table->setItem(y, 5, item_date);
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        connect(button_delete, &QPushButton::clicked, this, [this, donation]{ donationDelete(donation["rowid"].toLongLong()); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        connect(button_edit, &QPushButton::clicked, this, [this, donation]{ donationEdit(donation["rowid"].toLongLong()); });
        
        this->table->setCellWidget(y, 0, button_delete);
        this->table->setCellWidget(y, 1, button_edit);
    }
    QString sum = data_monthly_map[date_last.toString("yyyy-MM")].toString();
    this->table->setCellWidget(row_count-2, 3, new QLabel("<b>Σ: "+sum+"</b>"));
    
    this->table->resizeColumnsToContents();
}

void DonationsList::recreateView()
{
    showData();
}

void DonationsList::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
}

void DonationsList::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    recreateView();
}

void DonationsList::donationDelete(qlonglong rowid)
{
    qDebug() << "delete " << rowid;
}
void DonationsList::donationEdit(qlonglong rowid)
{
    DonationsEdit *edit = new DonationsEdit(rowid, this->db);
    connect(edit, &DonationsEdit::signalUpdate, this, &DonationsList::showData);
    
    QDialog *dialog = new QDialog();
    connect(dialog, &QDialog::finished, [this]{ showData(); });
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);
    
    dialog->exec();
}
void DonationsList::donationNew()
{
    qlonglong rowid = this->db->donationNewRow();
    
    showData();
    
    donationEdit(rowid);
}





DonationsEdit::DonationsEdit(qlonglong rowid, DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->rowid = rowid;
    this->db = db;
    
    setLayout(this->layout);
    
    this->date->setCalendarPopup(true);
    this->date->setDate(QDate::fromString(QDate::currentDate().toString(), "yyyy-MM-dd"));
    
    this->combo_currencies = new ComboCurrencies(this->db);
    
    //this->layout->addWidget(selector);
    this->layout->addWidget(this->button_name);
    this->layout->addWidget(new QLabel("date"));
    this->layout->addWidget(this->date);
    this->layout->addWidget(new QLabel("amount"));
    this->layout->addWidget(this->edit_amount);
    this->layout->addWidget(this->combo_currencies);
    this->layout->addWidget(new QLabel("memo"));
    this->layout->addWidget(this->edit_memo);
    this->layout->addWidget(new QLabel("TNT-code"));
    this->layout->addWidget(this->edit_tnt_code);
    //this->layout->addWidget(new QLabel("mandate reference"));
    //this->layout->addWidget(this->edit_mandate_reference);
    this->layout->addWidget(new QLabel("bank name"));
    this->layout->addWidget(this->edit_bank_name);
    
    QMap<QString,QVariant> data = db->donationSelect(rowid);
    
    this->rowid_person = data["rowid_people"].toLongLong();
    
    this->button_name->setText(data["name"].toString());
    this->date->setDate(QDate::fromString(data["date"].toString(), "yyyy-MM-dd"));
    this->edit_amount->setText(data["amount"].toString());
    this->edit_memo->setPlainText(data["memo"].toString());
    this->edit_tnt_code->setText(data["tnt_code"].toString());
    this->edit_mandate_reference->setText(data["mandate_reference"].toString());
    this->edit_bank_name->setText(data["bank_name"].toString());
    
    if (data["rowid_currencies"].toLongLong() != 0)
    {
        combo_currencies->setCurrentCurrencyRowid(data["rowid_currencies"].toLongLong());
    }
    
    connect(this->date, &QDateEdit::dateChanged, this, &DonationsEdit::saveData);
    connect(this->combo_currencies, &ComboCurrencies::currentTextChanged, this, &DonationsEdit::saveData);
    connect(this->button_name, &QPushButton::clicked, this, &DonationsEdit::changePerson);
    
    connect(this->edit_amount, &QLineEdit::textChanged, this, &DonationsEdit::saveData);
    connect(this->edit_memo, &QTextEdit::textChanged, this, &DonationsEdit::saveData);
    connect(this->edit_tnt_code, &QLineEdit::textChanged, this, &DonationsEdit::saveData);
    connect(this->edit_mandate_reference, &QLineEdit::textChanged, this, &DonationsEdit::saveData);
    connect(this->edit_bank_name, &QLineEdit::textChanged, this, &DonationsEdit::saveData);
}

void DonationsEdit::changePerson()
{
    PeopleSelector *selector = new PeopleSelector(this->db);
    connect(selector, &PeopleSelector::personSelected, this, &DonationsEdit::onSavePerson);
    
    this->dialog_select_person = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    this->dialog_select_person->setLayout(layout_dialog);
    layout_dialog->addWidget(selector);
    
    this->dialog_select_person->exec();
}
void DonationsEdit::onSavePerson(qlonglong rowid, QString name)
{
    this->rowid_person = rowid;
    this->person_name = name;
    
    this->dialog_select_person->close();
    
    saveData();
    
    this->button_name->setText(name);
}

void DonationsEdit::saveData()
{
    QMap<QString,QVariant> data;
    data["rowid_people"] = this->rowid_person;
    data["person_name"] = this->person_name;
    data["amount"] = this->edit_amount->text();
    data["rowid_currencies"] = this->combo_currencies->currentCurrencyRowid();
    data["date"] = this->date->date().toString("yyyy-MM-dd");
    data["memo"] = this->edit_memo->toPlainText();
    data["tnt_code"] = this->edit_tnt_code->text();
    data["bank_name"] = this->edit_bank_name->text();
    data["mandate"] = this->edit_mandate_reference->text();
    
    this->db->donationUpdate(this->rowid, data);
    
    //emit signalUpdate();
}
