#include "currencies.h"

CurrenciesList::CurrenciesList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , table (new QTableWidget)
    , combo_default_currency (new QComboBox)
{
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_currency = new QPushButton("add new Currency");
    //connect(button_new_currency, &QPushButton::clicked, this, &CurrenciesList::onEditButtonClicked);
    
    //this->layout->addWidget(new QLabel("default Currency"));
    //this->layout->addWidget(this->combo_default_currency);
    this->layout->addWidget(new QLabel("• The default-Currency should have an Exchange-Rate of 1"));
    this->layout->addWidget(new QLabel("• The Exchange-Rate of any other currency should be specified as a multiplicator in relation to the default Currency"));
    this->layout->addWidget(new QLabel("\t Example:\n\t If 1 USD = 0.91 EUR, and USD is your default Currency,\n\t USD would have an Exchange-Rate of 1,\n\t EUR would have an Exchange-Rate of 0.91"));
    this->layout->addWidget(new QLabel("• If no Currency has an Exchange-Rate of 1, the uppermost entry is taken as the default one"));
    this->layout->addWidget(new QLabel("• If more than one Currency has an Exchange-Rate of 1, the uppermost entry with an Exchange-Rate of 1 would be the default one"));
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new_currency);
    
    // iso-4217
    showData();
}

void CurrenciesList::showData()
{
    QList<QMap<QString,QVariant>> data = this->db->selectCurrencies();
    
    QStringList headers;
    headers << "" << "iso-4217 code" << "exchange rate" << "notes";
    this->table->setColumnCount(headers.length());
    this->table->setHorizontalHeaderLabels(headers);
    this->table->setRowCount(data.length());
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> cur = data.at(i);
        qlonglong rowid = cur["rowid"].toLongLong();
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setMaximumWidth(40);
        connect(button_edit, &QPushButton::clicked, this, [this, rowid]{ CurrenciesList::onEditButtonClicked(rowid); });
        
        this->table->setCellWidget(i, 0, button_edit);
        this->table->setItem(i, 1, new QTableWidgetItem(cur["code"].toString()));
        this->table->setItem(i, 2, new QTableWidgetItem(cur["exchange_rate"].toString()));
        this->table->setItem(i, 3, new QTableWidgetItem(cur["notes"].toString()));
    }
    
    this->table->resizeColumnsToContents();
}

void CurrenciesList::onEditButtonClicked(qlonglong rowid)
{
    CurrenciesEdit *edit = new CurrenciesEdit(rowid, this->db);
    connect(edit, &CurrenciesEdit::signalUpdate, this, &CurrenciesList::updateView);
    
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);
    
    dialog->exec();
}

void CurrenciesList::onNewButtonClicked()
{
    qlonglong rowid = this->db->insertCurrency();
    onEditButtonClicked(rowid);
    
    updateView();
}

void CurrenciesList::updateView()
{
    this->table->clear();
    showData();
}



CurrenciesEdit::CurrenciesEdit(qlonglong rowid, DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->rowid = rowid;
    this->db = db;
    
    setLayout(this->layout);
    
    this->layout->addWidget(new QLabel("ISO 4217 Code"));
    this->layout->addWidget(this->edit_code);
    this->layout->addWidget(new QLabel("Exchange Rate"));
    this->layout->addWidget(this->edit_exchange);
    this->layout->addWidget(new QLabel("Notes"));
    this->layout->addWidget(this->edit_notes);
    
    connect(this->edit_code, &QLineEdit::textChanged, this, &CurrenciesEdit::saveData);
    connect(this->edit_exchange, &QLineEdit::textChanged, this, &CurrenciesEdit::saveData);
    connect(this->edit_notes, &QTextEdit::textChanged, this, &CurrenciesEdit::saveData);
    
    QMap<QString,QVariant> data = db->selectCurrency(rowid);
    
    this->edit_code->setText(data["code"].toString());
    this->edit_exchange->setText(data["exchange_rate"].toString());
    this->edit_notes->setPlainText(data["notes"].toString());
}

void CurrenciesEdit::saveData()
{
    QString code = this->edit_code->text();
    QString exchange = this->edit_exchange->text();
    QString notes = this->edit_notes->toPlainText();
    
    this->db->updateCurrency(this->rowid, code, exchange, notes);
    emit signalUpdate();
}
