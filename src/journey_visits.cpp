#include "journey_visits.h"

JourneyVisits::JourneyVisits(qlonglong rowid_journey, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table (new QTableWidget)
    , layout (new QVBoxLayout)
{
    this->rowid_journey = rowid_journey;
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_visit = new QPushButton("add visited person");
    connect(button_new_visit, &QPushButton::clicked, this, &JourneyVisits::addNewVisit);
    
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new_visit);
    
    loadData();
}

void JourneyVisits::loadData()
{
    QStringList header;
    header << "" << "" << "name" << "date" << "notes";
    this->table->setColumnCount(header.length());
    this->table->setHorizontalHeaderLabels(header);
    
    QList<QMap<QString,QVariant>> data = this->db->selectVisitsForJourney(this->rowid_journey);
    this->table->setRowCount(data.length());
    
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
        
        this->table->setCellWidget(i, 0, button_delete);
        this->table->setCellWidget(i, 1, button_edit);
        
        this->table->setItem(i, 2, new QTableWidgetItem(journey["name"].toString()));
        this->table->setItem(i, 3, new QTableWidgetItem(QDate::fromString(journey["date"].toString(), "yyyy-MM-dd").toString()));
        this->table->setItem(i, 4, new QTableWidgetItem(journey["notes"].toString()));
    }
    
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->table->resizeColumnsToContents();
}

void JourneyVisits::addNewVisit()
{
    qlonglong rowid_visits = this->db->insertVisit(this->rowid_journey);
    
    this->table->clear();
    loadData();
    
    editVisit(rowid_visits);
}

void JourneyVisits::editVisit(qlonglong rowid_visits)
{
    JourneyVisitsEdit *edit = new JourneyVisitsEdit(rowid_visits, this->db);
    
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
        
        this->table->clear();
        loadData();
    }
}




JourneyVisitsEdit::JourneyVisitsEdit(qlonglong rowid_visits, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , edit_name (new QLineEdit)
    , edit_date (new QDateEdit)
    , edit_notes (new QTextEdit)
{
    this->rowid_visits = rowid_visits;
    this->db = db;
    
    setLayout(this->layout);
    
    this->edit_date->setCalendarPopup(true);
    
    this->layout->addWidget(new QLabel("name"));
    this->layout->addWidget(this->edit_name);
    this->layout->addWidget(new QLabel("date"));
    this->layout->addWidget(this->edit_date);
    this->layout->addWidget(new QLabel("notes"));
    this->layout->addWidget(this->edit_notes);
    
    QMap<QString,QVariant> data = this->db->selectVisit(this->rowid_visits);
    this->edit_name->setText(data["name"].toString());
    this->edit_date->setDate(QDate::fromString(data["date"].toString(), "yyyy-MM-dd"));
    this->edit_notes->setText(data["notes"].toString());
}

void JourneyVisitsEdit::saveData()
{
    QString name = this->edit_name->text();
    QString date = QDate::fromString(this->edit_date->date().toString(), "yyyy-MM-dd").toString();
    QString notes = this->edit_notes->toPlainText();
    
    //this->db->updateVisit(this->rowid_visits, this->rowid_journey, name, date, notes);
    
}
