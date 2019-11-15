#ifndef CURRENCIES_H
#define CURRENCIES_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include "db_adapter.h"

class Currencies : public QWidget
{
    Q_OBJECT
public:
    explicit Currencies(DbAdapter *db, QWidget *parent = nullptr);
    
    void showData();
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    QTableWidget *table;
    QComboBox *combo_default_currency;
    
signals:
    
public slots:
};

#endif // CURRENCIES_H
