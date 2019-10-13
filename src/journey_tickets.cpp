#include "journey_tickets.h"

JourneyTickets::JourneyTickets(qlonglong rowid_journey, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table (new QTableWidget)
    , layout (new QVBoxLayout)
{
    this->rowid_journey = rowid_journey;
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_visit = new QPushButton("add ticket");
    connect(button_new_visit, &QPushButton::clicked, this, &JourneyTickets::addNewTicket);
    
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new_visit);
    
    loadData();
}

void JourneyTickets::loadData()
{
    QList<QMap<QString,QVariant>> data = this->db->selectTicketsForJourney(this->rowid_journey);
    
    this->table->setRowCount(data.length());
    
    QStringList header;
    header << "" << "" << "name" << "cost" << "settled" << "notes";
    this->table->setColumnCount(header.length());
    this->table->setHorizontalHeaderLabels(header);
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> ticket = data.at(i);
        
        qlonglong rowid = ticket["rowid"].toLongLong();
        QString name = ticket["name"].toString();
        
        QPushButton *button_delete = new QPushButton;
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ JourneyTickets::deleteTicket(rowid, name); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        connect(button_edit, &QPushButton::clicked, this, [this, rowid]{ JourneyTickets::editTicket(rowid); });
        
        this->table->setCellWidget(i, 0, button_delete);
        this->table->setCellWidget(i, 1, button_edit);
        
        this->table->setItem(i, 2, new QTableWidgetItem(ticket["name"].toString()));
        
        QTableWidgetItem *cost = new QTableWidgetItem(ticket["cost"].toString());
        cost->setData(Qt::TextAlignmentRole,int(Qt::AlignRight|Qt::AlignVCenter));
        this->table->setItem(i, 3, cost);
        
        if (ticket["flag_settled"].toInt() == 1)
        {
            this->table->setItem(i, 4, new QTableWidgetItem("yes"));
        }
        else
        {
            this->table->setItem(i, 4, new QTableWidgetItem("not yet"));
        }
        
        this->table->setItem(i, 5, new QTableWidgetItem(ticket["notes"].toString()));
    }
    
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->table->resizeColumnsToContents();
}

void JourneyTickets::addNewTicket()
{
    qlonglong rowid = this->db->insertTicket(this->rowid_journey);
    
    this->table->clear();
    loadData();
    
    editTicket(rowid);
}

void JourneyTickets::editTicket(qlonglong rowid)
{
    JourneyTicketsEdit *edit = new JourneyTicketsEdit(rowid, this->db);
    connect(edit, &JourneyTicketsEdit::dataSaved, this, &JourneyTickets::reloadData);
    
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);
    
    dialog->exec();
}

void JourneyTickets::deleteTicket(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Delete "+name, "Really delete Ticket \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteTicket(rowid);
        
        this->table->clear();
        loadData();
    }
}

void JourneyTickets::reloadData()
{
    this->table->clear();
    loadData();
}



JourneyTicketsEdit::JourneyTicketsEdit(qlonglong rowid, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , edit_name (new QLineEdit)
    , edit_cost (new QDoubleSpinBox)
    , edit_flag_settled (new QCheckBox)
    , edit_notes (new QTextEdit)
{
    this->rowid = rowid;
    this->db = db;
    
    setLayout(this->layout);
    
    edit_cost->setMaximum(100000);
    
    this->layout->addWidget(new QLabel("name"));
    this->layout->addWidget(edit_name);
    this->layout->addWidget(new QLabel("cost"));
    this->layout->addWidget(edit_cost);
    this->layout->addWidget(new QLabel("settled?"));
    this->layout->addWidget(edit_flag_settled);
    this->layout->addWidget(new QLabel("notes"));
    this->layout->addWidget(edit_notes);
    
    QMap<QString,QVariant> data = this->db->selectTicket(this->rowid);
    
    edit_name->setText(data["name"].toString());
    edit_cost->setValue(data["cost"].toDouble());
    if (data["flag_settled"].toInt() == 1)
    {
        edit_flag_settled->setChecked(true);
    }
    edit_notes->setText(data["notes"].toString());
    
    connect(edit_name, &QLineEdit::textChanged, this, &JourneyTicketsEdit::saveData);
    connect(edit_cost, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &JourneyTicketsEdit::saveDataArg);
    connect(edit_flag_settled, &QCheckBox::stateChanged, this, &JourneyTicketsEdit::saveData);
    connect(edit_notes, &QTextEdit::textChanged, this, &JourneyTicketsEdit::saveData);
}

void JourneyTicketsEdit::saveData()
{
    QString name = this->edit_name->text();
    double cost = this->edit_cost->value();
    bool flag_settled = this->edit_flag_settled->isChecked();
    QString notes = this->edit_notes->toPlainText();
    
    this->db->updateTicket(this->rowid, name, cost, flag_settled, notes);
    
    emit dataSaved();
}

void JourneyTicketsEdit::saveDataArg(double /* just for compat */)
{
    saveData();
}
