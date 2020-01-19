#ifndef DONATIONSIMPORTPROGRESS_H
#define DONATIONSIMPORTPROGRESS_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QVBoxLayout>

#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>

#include "db_adapter.h"

class DonationsImportProgress : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsImportProgress(DbAdapter *db, QList<QMap<QString,QString>> data, QMap<QString,QString> data_map, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    //QGridLayout *layout = new QGridLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    
    QTextEdit *edit_log = new QTextEdit;
    
signals:
    
};

#endif // DONATIONSIMPORTPROGRESS_H
