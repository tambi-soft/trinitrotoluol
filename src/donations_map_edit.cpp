#include "donations_map_edit.h"

DonationsMapEdit::DonationsMapEdit(DbAdapter *db, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    
    this->help = new GrowingTextEdit;
    this->help->loadTextFromAssets(":help_import_map");
    
    this->layout->insertWidget(0, this->help);
    
    //showData();
}

void DonationsMapEdit::showData()
{
    recreateView();
    
    this->grid->addWidget(new QLabel("<b>Name</b>"), 1, 2);
    this->grid->addWidget(new QLabel("<b>TNT Memo</b>"), 1, 3);
    
    QList<QMap<QString,QVariant>> data = this->db->personSelectDonationsMap();
    
    for (int i=0; i < data.length(); i++)
    {
        QString name = data.at(i)["name"].toString();
        qlonglong rowid_people = data.at(i)["rowid_people"].toLongLong();
        QString tnt_name = data.at(i)["tnt_name"].toString();
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        connect(button_delete, &QPushButton::clicked, this, [this, rowid_people, name]{ DonationsMapEdit::onDeleteButtonClicked(rowid_people, name); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        connect(button_edit, &QPushButton::clicked, this, [this, tnt_name]{ DonationsMapEdit::onEditButtonClicked(tnt_name); });
        
        this->grid->addWidget(button_delete, i+2, 0);
        this->grid->addWidget(button_edit, i+2, 1);
        
        this->grid->addWidget(new QLabel(name), i+2, 2);
        this->grid->addWidget(new QLabel(tnt_name), i+2, 3);
    }
}

void DonationsMapEdit::onDeleteButtonClicked(qlonglong rowid_people, QString name)
{
    int reply = QMessageBox::question(this, "Delete Mapping for "+name, "Really delete Mapping for \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->personDeleteDonationsMap(rowid_people);
        
        showData();
    }
}

void DonationsMapEdit::onEditButtonClicked(QString tnt_name)
{
    this->tnt_name = tnt_name;
    
    PeopleSelector *selector = new PeopleSelector(this->db);
    selector->setDescription("Who is <b>"+tnt_name+"</b> in your database?");
    connect(selector, &PeopleSelector::personSelected, this, &DonationsMapEdit::onPersonSelected);
    
    this->dialog_select_person = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    this->dialog_select_person->setLayout(layout_dialog);
    layout_dialog->addWidget(selector);
    
    this->dialog_select_person->exec();
}

void DonationsMapEdit::onPersonSelected(qlonglong rowid, QString name)
{
    this->db->personUpdateDonationsMap(rowid, this->tnt_name);
    
    this->dialog_select_person->close();
    
    showData();
}

void DonationsMapEdit::hideEvent(QHideEvent */**event**/)
{
    
}

void DonationsMapEdit::showEvent(QShowEvent */**event**/)
{
    showData();
}
