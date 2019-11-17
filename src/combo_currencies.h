#ifndef COMBOCURRENCIES_H
#define COMBOCURRENCIES_H

#include <QObject>
#include <QWidget>
#include <QComboBox>

#include "db_adapter.h"

class ComboCurrencies : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboCurrencies(DbAdapter *db, QComboBox *parent = nullptr);
    
    qlonglong currentCurrencyRowid();
    QString currentCurrencyCode();
    
    void setCurrentCurrencyRowid(qlonglong rowid);
    void setCurrentCurrencyCode(QString code);
    
private:
    QList<QMap<QString,QVariant>> currencies;
    qlonglong current_currency_rowid;
    QString current_currency_code;
    QMap<qlonglong,QString> map_rowid_to_code;
    QMap<int,QString> map_combo_id_to_code;
    QMap<QString,qlonglong> map_code_to_rowid;
    QMap<QString,int> map_code_to_comboid;
    
    void currencyChanged(QString code);
    
signals:
    
public slots:
};

#endif // COMBOCURRENCIES_H
