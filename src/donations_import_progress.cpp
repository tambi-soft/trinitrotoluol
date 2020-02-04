#include "donations_import_progress.h"

DonationsImportProgress::DonationsImportProgress(DbAdapter *db, QList<QMap<QString,QString>> data, QMap<QString,QString> data_map, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    
    QProgressBar *progressbar = new QProgressBar;
    progressbar->setRange(0, data.length());
    
    this->layout->addWidget(progressbar);
    this->layout->addWidget(edit_log);
    
    for (int i=0; i < data.length(); ++i)
    {
        progressbar->setValue(i);
        
        this->person_name = data.at(i)[data_map["person_name"]];
        QString amount = data.at(i)[data_map["amount"]];
        QString currency_code = data.at(i)[data_map["currency_code"]];
        
        QString date_src = data.at(i)[data_map["date"]].split(" ")[0];
        QString date = QDate::fromString(date_src, "M/d/yyyy").toString("yyyy-MM-dd");
        
        this->memo = data.at(i)[data_map["memo"]];
        this->tnt_code = data.at(i)[data_map["tnt_code"]];
        this->tnt_donor_code = data.at(i)[data_map["tnt_donor_code"]];
        
        if (this->db->donationDoesEntryAlreadyExist(this->person_name, amount, date, this->memo, tnt_code))
        {
            
            edit_log->append("Entry ("+this->person_name+", "+amount+" "+currency_code+", "+date+", "+this->memo+") already exists, not importing again.");
            continue;
        }
        
        // ======================================================================
        // here we only get, if current entry is not recognized to be a duplicate
        // ======================================================================
        
        this->rowid_people = this->db->personRowidForTNTCode(data.at(i)[data_map["person_tnt_code"]]);
        this->rowid_currency = this->db->currencyROWIDForCode(currency_code);
        
        // check, if person already exists in database
        if (this->rowid_people < 1)
        {
            edit_log->append("Person not recognized in the database: "+this->person_name+" ("+this->memo+"). please create this person now!");
            
            selectPerson(this->person_name, this->memo);
        }
        
        // check, if currency already exists in database
        if (this->rowid_currency < 1)
        {
            edit_log->append("Currency "+currency_code+" not recognized in the database. Please create this currency now!");
            
            
        }
        
        // ======================================================================
        // all checks passed, now we are ready to save this entry in the database
        // ======================================================================
        QMap<QString,QVariant> insert_row_data;
        insert_row_data["rowid_people"] = this->rowid_people;
        insert_row_data["person_name"] = person_name;
        insert_row_data["amount"] = amount;
        insert_row_data["rowid_currencies"] = this->rowid_currency;
        insert_row_data["date"] = date;
        insert_row_data["memo"] = memo;
        insert_row_data["tnt_code"] = tnt_code;
        
        this->db->donationInsert(insert_row_data);
    }
    
    progressbar->setValue(data.length());
}

void DonationsImportProgress::selectPerson(QString name, QString memo)
{
    PeopleSelector *selector = new PeopleSelector(this->db);
    connect(selector, &PeopleSelector::personSelected, this, &DonationsImportProgress::onPersonSelected);
    
    this->dialog_select_person = new QDialog();
    QGridLayout *layout_dialog = new QGridLayout;
    this->dialog_select_person->setLayout(layout_dialog);
    
    QPushButton *button_person_create = new QPushButton("Create new Person");
    
    QGroupBox *group_person_select = new QGroupBox("Select an Existing Person");
    QGroupBox *group_person_create = new QGroupBox("Create new Person");
    
    QVBoxLayout *layout_group_select = new QVBoxLayout;
    QVBoxLayout *layout_group_create = new QVBoxLayout;
    
    group_person_select->setLayout(layout_group_select);
    group_person_create->setLayout(layout_group_create);
    
    layout_group_select->addWidget(selector);
    layout_group_create->addWidget(button_person_create);
    
    
    layout_dialog->addWidget(new QLabel("Person not recognized in the database:<br><b>"+name+" ("+memo+").</b><br>Please select manually or create a new person!"), 0, 0, 1, 3, Qt::AlignCenter);
    layout_dialog->addWidget(group_person_select, 2, 0);
    layout_dialog->addWidget(new QLabel("... or ..."), 2, 1);
    layout_dialog->addWidget(group_person_create, 2, 2);
    
    this->dialog_select_person->exec();
}

void DonationsImportProgress::onPersonSelected(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Select Person", "Do you really want to associate <br>"+this->person_name+" ("+this->memo+")</b> from tnt<br>With "+name+"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        if (this->tnt_donor_code.toInt() != 999)
        {
            this->db->personInsertTNTID(rowid, this->tnt_donor_code);
        }
        this->rowid_people = rowid;
        
        this->dialog_select_person->close();
    }
}
