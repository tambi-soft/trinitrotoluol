#include "journey_tickets.h"

JourneyTickets::JourneyTickets(qlonglong rowid_journey, DbAdapter *db, GridWidget *parent) : GridWidget(parent)
{
    this->rowid_journey = rowid_journey;
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_visit = new QPushButton("add ticket");
    connect(button_new_visit, &QPushButton::clicked, this, &JourneyTickets::addNewTicket);
    
    this->layout->addWidget(button_new_visit);
    
    showData();
}

void JourneyTickets::showData()
{
    deleteView();
    
    QList<QMap<QString,QVariant>> data = this->db->selectTicketsForJourney(this->rowid_journey);
    
    this->grid->addWidget(new QLabel("<b>Name</b>"), 0, 2);
    this->grid->addWidget(new QLabel("<b>Cost</b>"), 0, 3);
    this->grid->addWidget(new QLabel("<b>Currency</b>"), 0, 4);
    this->grid->addWidget(new QLabel("<b>Settled</b>"), 0, 5);
    this->grid->addWidget(new QLabel("<b>Notes</b>"), 0, 6);
    
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
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_edit, i+1, 1);
        
        this->grid->addWidget(new QLabel(ticket["name"].toString()), i+1, 2);
        
        QLabel *cost = new QLabel(ticket["cost"].toString());
        //cost->setData(Qt::TextAlignmentRole,int(Qt::AlignRight|Qt::AlignVCenter));
        this->grid->addWidget(cost, i+1, 3);
        
        this->grid->addWidget(new QLabel(ticket["currency_code"].toString()), i+1, 4);
        
        if (ticket["flag_settled"].toInt() == 1)
        {
            this->grid->addWidget(new QLabel("yes"), i+1, 5);
        }
        else
        {
            this->grid->addWidget(new QLabel("not yet"), i+1, 5);
        }
        
        this->grid->addWidget(new QLabel(ticket["notes"].toString()), i+1, 6);
    }
}

void JourneyTickets::addNewTicket()
{
    qlonglong rowid = this->db->insertTicket(this->rowid_journey);
    
    showData();
    
    editTicket(rowid);
}

void JourneyTickets::editTicket(qlonglong rowid)
{
    JourneyTicketsEdit *edit = new JourneyTicketsEdit(rowid, this->db);
    connect(edit, &JourneyTicketsEdit::dataSaved, this, &JourneyTickets::showData);
    
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
        
        showData();
    }
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
    
    edit_cost->setMinimum(0);
    edit_cost->setMaximum(1000000);
    
    this->combo_currencies = new ComboCurrencies(this->db);
    
    this->layout->addWidget(new QLabel("name"));
    this->layout->addWidget(edit_name);
    this->layout->addWidget(new QLabel("cost"));
    this->layout->addWidget(edit_cost);
    this->layout->addWidget(new QLabel("currency"));
    this->layout->addWidget(this->combo_currencies);
    this->layout->addWidget(new QLabel("settled?"));
    this->layout->addWidget(edit_flag_settled);
    this->layout->addWidget(new QLabel("notes"));
    this->layout->addWidget(edit_notes);
    
    QMap<QString,QVariant> data = this->db->selectTicket(this->rowid);
    
    edit_name->setText(data["name"].toString());
    edit_cost->setValue(data["cost"].toDouble());
    if (data["rowid_currency"].toLongLong() != 0)
    {
        combo_currencies->setCurrentCurrencyRowid(data["rowid_currency"].toLongLong());
    }
    if (data["flag_settled"].toInt() == 1)
    {
        edit_flag_settled->setChecked(true);
    }
    edit_notes->setText(data["notes"].toString());
    
    connect(edit_name, &QLineEdit::textChanged, this, &JourneyTicketsEdit::saveData);
    connect(edit_cost, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &JourneyTicketsEdit::saveDataArg);
    //connect(combo_currencies, qOverload<int>(&ComboCurrencies::currentIndexChanged), this, &JourneyTicketsEdit::saveDataInt);
    connect(combo_currencies, &QComboBox::currentTextChanged, this, &JourneyTicketsEdit::saveData);
    
    connect(edit_flag_settled, &QCheckBox::stateChanged, this, &JourneyTicketsEdit::saveData);
    connect(edit_notes, &QTextEdit::textChanged, this, &JourneyTicketsEdit::saveData);
}

void JourneyTicketsEdit::saveData()
{
    QString name = this->edit_name->text();
    double cost = this->edit_cost->value();
    qlonglong rowid_currency = this->combo_currencies->currentCurrencyRowid();
    bool flag_settled = this->edit_flag_settled->isChecked();
    QString notes = this->edit_notes->toPlainText();
    
    this->db->updateTicket(this->rowid, name, cost, rowid_currency, flag_settled, notes);
    
    emit dataSaved();
}

void JourneyTicketsEdit::saveDataArg(double /* just for compat */)
{
    saveData();
}

void JourneyTicketsEdit::saveDataInt(int /* just for compat */)
{
    saveData();
}
