#include "expenses_list.h"

ExpensesList::ExpensesList(DbAdapter *db, GridWidget *parent)
    : GridWidget(parent)
{
    this->db = db;
    
    QPushButton *button_new = new QPushButton("add new expenses");
    connect(button_new, &QPushButton::clicked, this, &ExpensesList::onNewButtonClicked);
    
    this->layout->addWidget(button_new);
}

void ExpensesList::showData()
{
    recreateView();
    
    this->grid->addWidget(new QLabel("<b>Name</b>"), 0, 2);
    this->grid->addWidget(new QLabel("<b>Amount</b>"), 0, 3);
    this->grid->addWidget(new QLabel("<b>Cost One</b>"), 0, 4);
    this->grid->addWidget(new QLabel("<b>Currency</b>"), 0, 5);
    this->grid->addWidget(new QLabel("<b>Sum</b>"), 0, 6);
    this->grid->addWidget(new QLabel("<b>Date</b>"), 0, 7);
    
    QList<QMap<QString,QVariant>> data = this->db->selectExpenses();
    
    for (int i=0; i < data.length(); ++i)
    {
        qlonglong rowid = data.at(i)["rowid"].toLongLong();
        QString name = data.at(i)["name"].toString();
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        //button_delete->setMaximumWidth(40);
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ ExpensesList::onDeleteButtonClicked(rowid, name); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setMaximumWidth(40);
        connect(button_edit, &QPushButton::clicked, this, [this, rowid]{ ExpensesList::onEditButtonClicked(rowid); });
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_edit, i+1, 1);
        
        this->grid->addWidget(new QLabel(name), i+1, 2);
        this->grid->addWidget(new QLabel(data.at(i)["amount"].toString()), i+1, 3);
        this->grid->addWidget(new QLabel(data.at(i)["cost_one"].toString()), i+1, 4);
        this->grid->addWidget(new QLabel(data.at(i)["currency_code"].toString()), i+1, 5);
        
        double sum = data.at(i)["cost_one"].toDouble() *
                data.at(i)["amount"].toDouble();
        this->grid->addWidget(new QLabel(QString::number(sum)), i+1, 6);
        
        this->grid->addWidget(new QLabel(data.at(i)["date"].toString()), i+1, 7);
    }
}

void ExpensesList::onDeleteButtonClicked(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Delete "+name, "Really delete \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteExpense(rowid);
        
        showData();
    }
}

void ExpensesList::onEditButtonClicked(qlonglong rowid)
{
    ExpensesEdit *edit = new ExpensesEdit(rowid, this->db);
    connect(edit, &ExpensesEdit::signalUpdate, this, &ExpensesList::showData);
    
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);
    
    dialog->exec();
}

void ExpensesList::onNewButtonClicked()
{
    qlonglong rowid = this->db->insertExpense();
    showData();
    onEditButtonClicked(rowid);
}

void ExpensesList::showEvent(QShowEvent */** event **/)
{
    showData();
}


ExpensesEdit::ExpensesEdit(qlonglong rowid, DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->rowid = rowid;
    this->db = db;
    setLayout(this->layout);
    
    this->combo_currency = new ComboCurrencies(db);
    
    this->layout->addWidget(new QLabel("name"));
    this->layout->addWidget(this->edit_name);
    this->layout->addWidget(new QLabel("amount"));
    this->layout->addWidget(this->edit_amount);
    this->layout->addWidget(new QLabel("cost one"));
    this->layout->addWidget(this->edit_cost_one);
    this->layout->addWidget(new QLabel("currency"));
    this->layout->addWidget(this->combo_currency);
    this->layout->addWidget(new QLabel("date"));
    this->layout->addWidget(this->edit_date);
    this->layout->addWidget(new QLabel("already settled?"));
    this->layout->addWidget(this->check_settled);
    this->layout->addWidget(new QLabel("notes"));
    this->layout->addWidget(this->edit_notes);
    
    QMap<QString,QVariant> data = this->db->selectExpense(this->rowid);
    this->rowid_currency = data["rowid_currencoy"].toLongLong();
    
    
    this->edit_name->setText(data["name"].toString());
    this->edit_amount->setText(data["amount"].toString());
    this->edit_cost_one->setText(data["cost_one"].toString());
    
    if (data["rowid_currency"].toLongLong() != 0)
    {
        this->combo_currency->setCurrentCurrencyRowid(data["rowid_currency"].toLongLong());
    }
    
    this->edit_date->setCalendarPopup(true);
    this->edit_date->setDate(QDate::fromString(data["date"].toString(), "yyyy-MM-dd"));
    
    if (data["flag_settled"].toInt() > 0)
    {
        this->check_settled->setChecked(true);
    }
    this->edit_notes->setPlainText(data["notes"].toString());
    
    connect(this->edit_name, &QLineEdit::textChanged, this, &ExpensesEdit::saveData);
    connect(this->edit_amount, &QLineEdit::textChanged, this, &ExpensesEdit::saveData);
    connect(this->edit_cost_one, &QLineEdit::textChanged, this, &ExpensesEdit::saveData);
    connect(this->combo_currency, &QComboBox::currentTextChanged, this, &ExpensesEdit::saveData);
    connect(this->edit_date, &QDateEdit::dateChanged, this, &ExpensesEdit::saveData);
    connect(this->check_settled, &QCheckBox::stateChanged, this, &ExpensesEdit::saveData);
    connect(this->edit_notes, &QPlainTextEdit::textChanged, this, &ExpensesEdit::saveData);
}

void ExpensesEdit::saveData()
{
    QMap<QString,QVariant> data;
    data["name"] = this->edit_name->text();
    data["amount"] = this->edit_amount->text();
    data["cost_one"] = this->edit_cost_one->text();
    data["date"] = this->edit_date->date().toString("yyyy-MM-dd");
    data["flag_settled"] = this->check_settled->isChecked();
    data["notes"] = this->edit_notes->toPlainText();
    
    this->rowid_currency = this->combo_currency->currentCurrencyRowid();
    //this->rowid_currency =  this->currencies.at(this->combo_currency->currentIndex())["rowid"].toLongLong();
    this->db->updateExpense(this->rowid, this->rowid_currency, data);
    
    emit signalUpdate();
}
