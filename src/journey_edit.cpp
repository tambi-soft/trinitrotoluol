#include "journey_edit.h"

/*
JourneyEdit::JourneyEdit(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->db = db;
    
    setLayout(this->layout);
}
*/

JourneyEdit::JourneyEdit(qlonglong rowid, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->rowid = rowid;
    this->db = db;
    
    setLayout(this->layout);
    
    drawGUI();
    loadData();
}

void JourneyEdit::drawGUI()
{
    this->edit_name = new QLineEdit;
    this->edit_date_from = new QDateEdit;
    this->edit_date_to = new QDateEdit;
    this->edit_notes = new QTextEdit;
    
    this->edit_date_from->setDate(QDate::currentDate());
    this->edit_date_to->setDate(QDate::currentDate());
    
    this->edit_date_from->setCalendarPopup(true);
    this->edit_date_to->setCalendarPopup(true);
    
    this->layout->addWidget(new QLabel("name of the journey"));
    this->layout->addWidget(edit_name);
    this->layout->addWidget(new QLabel("date start"));
    this->layout->addWidget(edit_date_from);
    this->layout->addWidget(new QLabel("date end"));
    this->layout->addWidget(edit_date_to);
    this->layout->addWidget(new QLabel("notes"));
    this->layout->addWidget(edit_notes);
    
    connect(this->edit_name, &QLineEdit::textChanged, this, &JourneyEdit::saveData);
    connect(this->edit_date_from, &QDateEdit::dateChanged, this, &JourneyEdit::saveData);
    connect(this->edit_date_to, &QDateEdit::dateChanged, this, &JourneyEdit::saveData);
    connect(this->edit_notes, &QTextEdit::textChanged, this, &JourneyEdit::saveData);
    
    // BEGIN: groups for visits and tickets areas
    QWidget *widget_groups = new QWidget;
    QHBoxLayout *layout_groups = new QHBoxLayout;
    widget_groups->setLayout(layout_groups);
    layout_groups->setMargin(0);
    this->layout->addWidget(widget_groups);
    
    QGroupBox *group_visits = new QGroupBox("Visited People");
    QGroupBox *group_tickets = new QGroupBox("Tickets");
    
    JourneyVisits *visits = new JourneyVisits(this->rowid, this->db);
    QVBoxLayout *layout_visits = new QVBoxLayout();
    layout_visits->setMargin(0);
    layout_visits->addWidget(visits);
    group_visits->setLayout(layout_visits);
    
    JourneyTickets *tickets = new JourneyTickets(this->rowid, this->db);
    QVBoxLayout *layout_tickets = new QVBoxLayout();
    layout_tickets->setMargin(0);
    layout_tickets->addWidget(tickets);
    group_tickets->setLayout(layout_tickets);
    
    layout_groups->addWidget(group_visits);
    layout_groups->addWidget(group_tickets);
    // END: groups for visits and tickets areas
    
    //this->layout->addStretch();
}

void JourneyEdit::loadData()
{
    QMap<QString,QVariant> data = this->db->selectJourney(this->rowid);
    this->edit_name->setText(data["name"].toString());
    
    this->edit_date_from->setDate(QDate::fromString(data["date_from"].toString(), "yyyy-MM-dd"));
    this->edit_date_to->setDate(QDate::fromString(data["date_to"].toString(), "yyyy-MM-dd"));
    
    this->edit_notes->setText(data["notes"].toString());
    
}

void JourneyEdit::saveData()
{
    QString name = this->edit_name->text();
    QString date_from = this->edit_date_from->date().toString("yyyy-MM-dd");
    QString date_to = this->edit_date_to->date().toString("yyyy-MM-dd");
    QString notes = this->edit_notes->toPlainText();
    
    this->db->updateJourney(this->rowid, name, date_from, date_to, notes);
}
