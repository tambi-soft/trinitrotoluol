#include "currencies.h"

CurrenciesList::CurrenciesList(DbAdapter *db, GridWidget *parent) : GridWidget(parent)
    , combo_default_currency (new QComboBox)
{
    this->db = db;
    
    QPushButton *button_new_currency = new QPushButton("add new Currency");
    connect(button_new_currency, &QPushButton::clicked, this, &CurrenciesList::onNewButtonClicked);
    
    //this->layout->addWidget(new QLabel("default Currency"));
    //this->layout->addWidget(this->combo_default_currency);
    this->layout->insertWidget(0, new QLabel("• The default-Currency should have an Exchange-Rate of 1"));
    this->layout->insertWidget(1, new QLabel("• The Exchange-Rate of any other currency should be specified as a multiplicator in relation to the default Currency"));
    this->layout->insertWidget(2, new QLabel("\t Example:\n\t If 1 USD = 0.91 EUR, and USD is your default Currency,\n\t USD would have an Exchange-Rate of 1,\n\t EUR would have an Exchange-Rate of 0.91"));
    this->layout->insertWidget(3, new QLabel("• If no Currency has an Exchange-Rate of 1, the uppermost entry is taken as the default one"));
    this->layout->insertWidget(4, new QLabel("• If more than one Currency has an Exchange-Rate of 1, the uppermost entry with an Exchange-Rate of 1 would be the default one"));
    this->layout->addWidget(button_new_currency);
    
    // iso-4217
    showData();
}

void CurrenciesList::showData()
{
    deleteView();
    
    QList<QMap<QString,QVariant>> data = this->db->selectCurrencies();
    
    this->grid->addWidget(new QLabel("<b>ISO-4217 Code</b>"), 0, 2);
    this->grid->addWidget(new QLabel("<b>Exchange Rate</b>"), 0, 3);
    this->grid->addWidget(new QLabel("<b>Notes</b>"), 0, 4);
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> cur = data.at(i);
        qlonglong rowid = cur["rowid"].toLongLong();
        QString code = cur["code"].toString();
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        connect(button_edit, &QPushButton::clicked, this, [this, rowid]{ CurrenciesList::onEditButtonClicked(rowid); });
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, code]{ CurrenciesList::onDeleteButtonClicked(rowid, code); });
        
        this->grid->addWidget(button_edit, i+1, 0);
        this->grid->addWidget(button_delete, i+1, 1);
        this->grid->addWidget(new QLabel(code), i+1, 2);
        this->grid->addWidget(new QLabel(cur["exchange_rate"].toString()), i+1, 3);
        this->grid->addWidget(new QLabel(cur["notes"].toString()), i+1, 4);
    }
}

void CurrenciesList::onEditButtonClicked(qlonglong rowid)
{
    CurrenciesEdit *edit = new CurrenciesEdit(rowid, this->db);
    connect(edit, &CurrenciesEdit::signalUpdate, this, &CurrenciesList::onUpdateSignaled);
    
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
    
    showData();
    
    onEditButtonClicked(rowid);
}

void CurrenciesList::onDeleteButtonClicked(qlonglong rowid, QString code)
{
    int reply = QMessageBox::question(this, "Delete "+code, "Really delete \""+code+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteCurrency(rowid);
        
        showData();
    }
}

void CurrenciesList::onUpdateSignaled()
{
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
