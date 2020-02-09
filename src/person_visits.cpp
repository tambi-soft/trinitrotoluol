#include "person_visits.h"

PersonVisits::PersonVisits(DbAdapter *db, qlonglong rowid, GridWidget *parent)
    : GridWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid;
    
    showData();
}

void PersonVisits::showData()
{
    recreateView();
    
    QList<QMap<QString,QVariant>> data = this->db->selectVisitsForPerson(this->rowid_person);
    
    this->grid->addWidget(new QLabel("<b>Journey Name</b>"), 0, 2);
    this->grid->addWidget(new QLabel("<b>Date</b>"), 0, 3);
    this->grid->addWidget(new QLabel("<b>Notes</b>"), 0, 4);
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> visit = data.at(i);
        qlonglong rowid = visit["rowid"].toLongLong();
        QString name = visit["journey_name"].toString();
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        button_delete->setToolTip("delete this entry");
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ PersonVisits::onDeleteButtonClicked(rowid, name); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setToolTip("edit this entry");
        connect(button_edit, &QPushButton::clicked, this, [this, rowid]{ PersonVisits::onEditButtonClicked(rowid); });
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_edit, i+1, 1);
        this->grid->addWidget(new QLabel(name), i+1, 2);
        this->grid->addWidget(new QLabel(visit["date"].toString()), i+1, 3);
        this->grid->addWidget(new QLabel(visit["notes"].toString()), i+1, 4);
    }
}

void PersonVisits::onDeleteButtonClicked(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Delete "+name, "Really delete \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteVisit(rowid);
        showData();
    }
}

void PersonVisits::onEditButtonClicked(qlonglong rowid)
{
    
}
