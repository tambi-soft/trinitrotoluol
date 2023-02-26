#ifndef DONATIONSRECEIPTGENERATOR_H
#define DONATIONSRECEIPTGENERATOR_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QRegExp>

#include <QDebug>

#include "db_adapter.h"

class DonationsReceiptGenerator : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsReceiptGenerator(DbAdapter *db, QWidget *parent = nullptr);
    
    QString numberToStringGerman(QString str_number);
    
private:
    DbAdapter *db;
    
    QLabel *label_test;
    void lineTestEdited(QString text);
    
signals:
    
};

#endif // DONATIONSRECEIPTGENERATOR_H
