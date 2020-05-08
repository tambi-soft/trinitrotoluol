#include "journey_visits_edit.h"

JourneyVisitsEdit::JourneyVisitsEdit(qlonglong rowid_visits, DbAdapter *db, QString date_hint, QWidget *parent) : QWidget(parent)
{
    this->rowid_visits = rowid_visits;
    this->db = db;
    
    initializeView(date_hint);
}

JourneyVisitsEdit::JourneyVisitsEdit(qlonglong rowid_visits, qlonglong rowid_person, DbAdapter *db, QString date_hint, QWidget *parent) : QWidget(parent)
{
    this->rowid_visits = rowid_visits;
    this->rowid_person = rowid_person;
    this->db = db;
    
    initializeView(date_hint);
}

void JourneyVisitsEdit::initializeView(QString date_hint)
{
    setLayout(this->layout);
    
    this->edit_date->setCalendarPopup(true);
    
    if (this->rowid_person == -1)
    {
        this->layout->addWidget(new QLabel("name"));
        this->layout->addWidget(this->button_name);
    }
    this->layout->addWidget(new QLabel("date"));
    this->layout->addWidget(this->edit_date);
    this->layout->addWidget(new QLabel("notes"));
    this->layout->addWidget(this->edit_notes);
    
    QMap<QString,QVariant> data = this->db->selectVisit(this->rowid_visits);
    
    // just set rowid_person, if we do not have it already
    if (this->rowid_person == -1)
    {
        this->rowid_person = data["rowid_people"].toLongLong();
    }
    
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
