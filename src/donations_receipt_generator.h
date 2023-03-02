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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDate>
#include <QDateEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

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
    
    QDateEdit *date_from;
    QDateEdit *date_to;
    
    QLineEdit *line_tex_input;
    QLineEdit *line_tex_output;
    
    QTableWidget *table;
    
    void selectDonorsFromDB();
    QList<QMap<QString,QVariant>> data;
    
    void lineTeXInputChanged(QString text);
    void lineTeXOutputChanged(QString text);
    
    void inputFileSelect();
    void outputFolderSelect();
    
    QStringList openInputTeXFile();
    
    void generateTexFiles();
    void generator(QString donorid, QString name, QString address, QString location, QString amount_sum, QString amount_string, QString date_from, QString date_to, QList<QMap<QString,QVariant>> list_details);
    
signals:
    
};

#endif // DONATIONSRECEIPTGENERATOR_H
