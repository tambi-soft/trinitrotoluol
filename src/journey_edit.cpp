#include "journey_edit.h"

JourneyEdit::JourneyEdit(qlonglong rowid, DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->rowid = rowid;
    this->db = db;
    
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    this->visits = new JourneyVisits(this->rowid, this->db, QDate::currentDate().toString("yyyy-MM-dd"));
    JourneyDetails *data = new JourneyDetails(this->rowid, this->db, this->visits);
    JourneyTickets *tickets = new JourneyTickets(this->rowid, this->db);
    
    QGroupBox *group_edit = new QGroupBox("Data");
    QGroupBox *group_visits = new QGroupBox("Visited People");
    QGroupBox *group_tickets = new QGroupBox("Tickets");
    
    QVBoxLayout *layout_edit = new QVBoxLayout;
    QVBoxLayout *layout_visits = new QVBoxLayout;
    QVBoxLayout *layout_tickets = new QVBoxLayout;
    
    group_edit->setLayout(layout_edit);
    group_visits->setLayout(layout_visits);
    group_tickets->setLayout(layout_tickets);
    
    layout_edit->addWidget(data);
    layout_visits->addWidget(this->visits);
    layout_tickets->addWidget(tickets);
    
    grid->addWidget(group_edit, 0, 0, 2, 1);
    grid->addWidget(group_visits, 0, 1);
    grid->addWidget(group_tickets, 1, 1);
}



JourneyDetails::JourneyDetails(qlonglong rowid, DbAdapter *db, JourneyVisits *visits, QWidget *parent)
    : QWidget(parent)
    , layout (new QGridLayout)
{
    this->rowid = rowid;
    this->db = db;
    this->visits = visits;
    
    setLayout(this->layout);
    this->layout->setMargin(0);
    
    drawGUI();
    loadData();
}

void JourneyDetails::drawGUI()
{
    this->edit_name = new QLineEdit(this);
    this->edit_date_from = new QDateEdit(this);
    this->edit_date_to = new QDateEdit(this);
    this->edit_notes = new QTextEdit(this);
    
    this->edit_date_from->setDate(QDate::currentDate());
    this->edit_date_to->setDate(QDate::currentDate());
    
    this->edit_date_from->setCalendarPopup(true);
    this->edit_date_to->setCalendarPopup(true);
    
    this->layout->addWidget(new QLabel("name of the journey"), 0, 0);
    this->layout->addWidget(edit_name, 1, 0);
    this->layout->addWidget(new QLabel("date start"), 2, 0);
    this->layout->addWidget(edit_date_from, 3, 0);
    this->layout->addWidget(new QLabel("date end"), 4, 0);
    this->layout->addWidget(edit_date_to, 5, 0);
    this->layout->addWidget(new QLabel("notes"), 6, 0);
    this->layout->addWidget(edit_notes, 7, 0);
    
    connect(this->edit_name, &QLineEdit::textChanged, this, &JourneyDetails::saveData);
    connect(this->edit_date_from, &QDateEdit::dateChanged, this, &JourneyDetails::saveData);
    connect(this->edit_date_to, &QDateEdit::dateChanged, this, &JourneyDetails::saveData);
    connect(this->edit_notes, &QTextEdit::textChanged, this, &JourneyDetails::saveData);
}

void JourneyDetails::loadData()
{
    blockSignals(true);
    this->edit_name->blockSignals(true);
    this->edit_date_from->blockSignals(true);
    this->edit_date_to->blockSignals(true);
    this->edit_notes->blockSignals(true);
    
    QMap<QString,QVariant> data = this->db->selectJourney(this->rowid);
    this->edit_name->setText(data["name"].toString());
    
    this->edit_date_from->setDate(QDate::fromString(data["date_from"].toString(), "yyyy-MM-dd"));
    this->edit_date_to->setDate(QDate::fromString(data["date_to"].toString(), "yyyy-MM-dd"));
    
    this->edit_notes->setText(data["notes"].toString());
    
    this->visits->setDateHint(this->edit_date_from->date().toString("yyyy-MM-dd"));
    
    blockSignals(false);
    this->edit_name->blockSignals(false);
    this->edit_date_from->blockSignals(false);
    this->edit_date_to->blockSignals(false);
    this->edit_notes->blockSignals(false);
}

void JourneyDetails::saveData()
{
    QString name = this->edit_name->text();
    QString date_from = this->edit_date_from->date().toString("yyyy-MM-dd");
    QString date_to = this->edit_date_to->date().toString("yyyy-MM-dd");
    QString notes = this->edit_notes->toPlainText();
    
    this->db->updateJourney(this->rowid, name, date_from, date_to, notes);
    
    this->visits->setDateHint(this->edit_date_from->date().toString("yyyy-MM-dd"));
}
