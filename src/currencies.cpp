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
}
