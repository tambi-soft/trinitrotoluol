#include "expenses_list.h"

ExpensesList::ExpensesList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
{
    this->db = db;
    setLayout(this->layout);
    
    QPushButton *button_new = new QPushButton("add new expenses");
    connect(button_new, &QPushButton::clicked, this, &ExpensesList::onNewButtonClicked);
    
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new);
}

void ExpensesList::loadData()
{
    QStringList headers;
    headers << "" << "" << "name" << "amount" << "cost one" << "currency" << "sum" << "date";
    
    QList<QMap<QString,QVariant>> data = this->db->selectExpenses();
    this->table->setRowCount(data.length());
    this->table->setColumnCount(headers.length());
    this->table->setHorizontalHeaderLabels(headers);
    
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
        
        this->table->setCellWidget(i, 0, button_delete);
        this->table->setCellWidget(i, 1, button_edit);
        
        this->table->setItem(i, 2, new QTableWidgetItem(name));
        this->table->setItem(i, 3, new QTableWidgetItem(data.at(i)["amount"].toString()));
        this->table->setItem(i, 4, new QTableWidgetItem(data.at(i)["cost_one"].toString()));
        this->table->setItem(i, 5, new QTableWidgetItem(data.at(i)["currency_code"].toString()));
        
        double sum = data.at(i)["cost_one"].toDouble() *
                data.at(i)["amount"].toDouble();
        this->table->setItem(i, 6, new QTableWidgetItem(
                                 QString::number(sum)
                                 ));
        
        this->table->setItem(i, 7, new QTableWidgetItem(data.at(i)["date"].toString()));
    }
    
    this->table->resizeColumnsToContents();
}

void ExpensesList::onDeleteButtonClicked(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Delete "+name, "Really delete \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteExpense(rowid);
        
        updateView();
    }
}

void ExpensesList::onEditButtonClicked(qlonglong rowid)
{
    ExpensesEdit *edit = new ExpensesEdit(rowid, this->db);
    connect(edit, &ExpensesEdit::signalUpdate, this, &ExpensesList::updateView);
    
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
    onEditButtonClicked(rowid);
    
    updateView();
}

void ExpensesList::updateView()
{
    this->table->clear();
    loadData();
}

void ExpensesList::showEvent(QShowEvent */** event **/)
{
    updateView();
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
    
    this->combo_currency->setCurrentCurrencyRowid(data["rowid_currency"].toLongLong());
    
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
