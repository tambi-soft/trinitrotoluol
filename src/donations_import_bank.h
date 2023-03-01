#ifndef DONATIONSIMPORTBANK_H
#define DONATIONSIMPORTBANK_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>
#include <QFile>
#include <QList>
#include <QMap>

#include <QDebug>

#include "db_adapter.h"
#include "config.h"
#include "parse_csv.h"

class DonationsImportBank : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsImportBank(Config *conf, DbAdapter *db, QWidget *parent = nullptr);
    
    void openCSV();
    void parseCSV();
    
private:
    DbAdapter *db;
    Config *conf;
    
    QTableWidget *table;
    QTextEdit *text;
    
    QList<QMap<QString,QString>> data;
    
    QMap<QString, QVariant> remapDataForDB(QMap<QString, QString> map);
    void writeValueToDB();
    
signals:
    
};

#endif // DONATIONSIMPORTBANK_H
