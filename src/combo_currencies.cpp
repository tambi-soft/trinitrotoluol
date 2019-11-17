#include "combo_currencies.h"

ComboCurrencies::ComboCurrencies(DbAdapter *db, QComboBox *parent) : QComboBox(parent)
{
    this->currencies = db->selectCurrencies();
    
    connect(this, &QComboBox::currentTextChanged, this, &ComboCurrencies::currencyChanged);
    
    QStringList currency_codes;
    for (int i=0; i < this->currencies.length(); ++i)
    {
        QMap<QString,QVariant> current_currency = this->currencies.at(i);
        currency_codes.append(current_currency["code"].toString());
        
        this->map_code_to_rowid[current_currency["code"].toString()] = current_currency["rowid"].toLongLong();
        this->map_rowid_to_code[current_currency["rowid"].toLongLong()] = current_currency["code"].toString();
        this->map_code_to_comboid[current_currency["code"].toString()] = i;
        this->map_combo_id_to_code[i] = current_currency["code"].toString();
        /*
        if (data["rowid_currency"].toLongLong() == current_currency["rowid"].toLongLong())
        {
            currency_code = current_currency["code"].toString();
        }
        */
    }
    addItems(currency_codes);
}

qlonglong ComboCurrencies::currentCurrencyRowid()
{
    return this->current_currency_rowid;
}

QString ComboCurrencies::currentCurrencyCode()
{
    return this->current_currency_code;
}

void ComboCurrencies::setCurrentCurrencyRowid(qlonglong rowid)
{
    this->current_currency_rowid = rowid;
    this->current_currency_code = this->map_rowid_to_code[rowid];
    
    int comboid = this->map_code_to_comboid[this->current_currency_code];
    blockSignals(true);
    setCurrentIndex(comboid);
    blockSignals(false);
}

void ComboCurrencies::setCurrentCurrencyCode(QString code)
{
    this->current_currency_code = code;
    this->current_currency_rowid = this->map_code_to_rowid[code];
    
    int comboid = this->map_code_to_comboid[code];
    blockSignals(true);
    setCurrentIndex(comboid);
    blockSignals(false);
}

void ComboCurrencies::currencyChanged(QString code)
{
    this->current_currency_code = code;
    this->current_currency_rowid = this->map_code_to_rowid[code];
}
