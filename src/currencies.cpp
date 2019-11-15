#include "currencies.h"

Currencies::Currencies(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , table (new QTableWidget)
    , combo_default_currency (new QComboBox)
{
    this->db = db;
    
    setLayout(this->layout);
    
    QPushButton *button_new_currency = new QPushButton("add new Currency");
    
    this->layout->addWidget(new QLabel("default Currency"));
    this->layout->addWidget(this->combo_default_currency);
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new_currency);
    
    // iso-4217
    showData();
}

void Currencies::showData()
{
    QList<QMap<QString,QVariant>> data = this->db->selectCurrencies();
    
    QStringList headers;
    headers << "iso-4217 code" << "exchange rate" << "notes";
    this->table->setColumnCount(headers.length());
    this->table->setHorizontalHeaderLabels(headers);
    this->table->setRowCount(data.length());
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> cur = data.at(i);
        
        this->table->setItem(i, 0, new QTableWidgetItem(cur["code"].toString()));
        this->table->setItem(i, 1, new QTableWidgetItem(cur["exchange_rate"].toString()));
        this->table->setItem(i, 2, new QTableWidgetItem(cur["notes"].toString()));
    }
    
    this->table->resizeColumnsToContents();
}
