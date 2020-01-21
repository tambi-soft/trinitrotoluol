#include "journey_visits.h"

JourneyVisits::JourneyVisits(qlonglong rowid_journey, DbAdapter *db, QString date_hint, GridWidget *parent) : GridWidget(parent)
{
    this->rowid_journey = rowid_journey;
    this->db = db;
    
    this->date_hint = date_hint;
    
    QPushButton *button_new_visit = new QPushButton("add visited person");
    connect(button_new_visit, &QPushButton::clicked, this, &JourneyVisits::addNewVisit);
    
    this->layout->addWidget(button_new_visit);
    
    showData();
}

void JourneyVisits::setDateHint(QString date)
{
    this->date_hint = date;
}

void JourneyVisits::showData()
{
    deleteView();
    
    this->grid->addWidget(new QLabel("<b>Name</b>"), 0, 2);
    this->grid->addWidget(new QLabel("<b>Date</b>"), 0, 3);
    this->grid->addWidget(new QLabel("<b>Notes</b>"), 0, 4);
    
    QList<QMap<QString,QVariant>> data = this->db->selectVisitsForJourney(this->rowid_journey);
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> journey = data.at(i);
        qlonglong rowid = journey["rowid"].toLongLong();
        QString name = journey["name"].toString();
        
        QPushButton *button_delete = new QPushButton;
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ JourneyVisits::deleteVisit(rowid, name); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        connect(button_edit, &QPushButton::clicked, this, [this, rowid]{ JourneyVisits::editVisit(rowid); });
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_edit, i+1, 1);
        
        this->grid->addWidget(new QLabel(journey["name"].toString()), i+1, 2);
        this->grid->addWidget(new QLabel(QDate::fromString(journey["date"].toString(), "yyyy-MM-dd").toString()), i+1, 3);
        this->grid->addWidget(new QLabel(journey["notes"].toString()), i+1, 4);
    }
}

void JourneyVisits::addNewVisit()
{
    qlonglong rowid_visits = this->db->insertVisit(this->rowid_journey);
    
    showData();
    
    editVisit(rowid_visits);
}

void JourneyVisits::editVisit(qlonglong rowid_visits)
{
    JourneyVisitsEdit *edit = new JourneyVisitsEdit(rowid_visits, this->db, this->date_hint);
    connect(edit, &JourneyVisitsEdit::signalReload, this, &JourneyVisits::showData);
    
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);
    
    dialog->exec();
}

void JourneyVisits::deleteVisit(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Delete "+name, "Really delete Visit-Entry \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteVisit(rowid);
        
        showData();
    }
}




JourneyVisitsEdit::JourneyVisitsEdit(qlonglong rowid_visits, DbAdapter *db, QString date_hint, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , button_name (new QPushButton)
    , edit_date (new QDateEdit)
    , edit_notes (new QTextEdit)
    , dialog_select_person (new QDialog)
{
    this->rowid_visits = rowid_visits;
    this->db = db;
    
    setLayout(this->layout);
    
    this->edit_date->setCalendarPopup(true);
    
    this->layout->addWidget(new QLabel("name"));
    this->layout->addWidget(this->button_name);
    this->layout->addWidget(new QLabel("date"));
    this->layout->addWidget(this->edit_date);
    this->layout->addWidget(new QLabel("notes"));
    this->layout->addWidget(this->edit_notes);
    
    QMap<QString,QVariant> data = this->db->selectVisit(this->rowid_visits);
    this->rowid_person = data["rowid_people"].toLongLong();
    
    this->button_name->setText(data["name"].toString());
    if (data["date"].toString().length() > 0)
    {
        this->edit_date->setDate(QDate::fromString(data["date"].toString(), "yyyy-MM-dd"));
    }
    else
    {
        this->edit_date->setDate(QDate::fromString(date_hint, "yyyy-MM-dd"));
    }
    this->edit_notes->setText(data["notes"].toString());
    
    connect(this->button_name, &QPushButton::clicked, this, &JourneyVisitsEdit::selectPerson);
    connect(this->edit_date, &QDateEdit::dateChanged, this, &JourneyVisitsEdit::saveData);
    connect(this->edit_notes, &QTextEdit::textChanged, this, &JourneyVisitsEdit::saveData);
}

void JourneyVisitsEdit::selectPerson()
{
    PeopleSelector *selector = new PeopleSelector(this->db);
    connect(selector, &PeopleSelector::personSelected, this, &JourneyVisitsEdit::onPersonSelected);
    
    this->dialog_select_person = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    this->dialog_select_person->setLayout(layout_dialog);
    layout_dialog->addWidget(selector);
    
    this->dialog_select_person->exec();
}

void JourneyVisitsEdit::onPersonSelected(qlonglong rowid, QString name)
{
    this->rowid_person = rowid;
    this->button_name->setText(name);
    
    this->dialog_select_person->close();
    
    saveData();
}

void JourneyVisitsEdit::saveData()
{
    QString date = this->edit_date->date().toString("yyyy-MM-dd");
    QString notes = this->edit_notes->toPlainText();
    
    this->db->updateVisit(this->rowid_visits, this->rowid_person, date, notes);
    
    emit signalReload();
}
