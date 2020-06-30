#ifndef VCARD_H
#define VCARD_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>

#include "db_adapter.h"

class VCardExport : public QWidget
{
    Q_OBJECT
public:
    explicit VCardExport(DbAdapter *db, QWidget *parent = nullptr);
    
    void selectNewFileDialog();
    
private:
    DbAdapter *db;
    
signals:
    
};



class VCardImport : public QWidget
{
    Q_OBJECT
public:
    explicit VCardImport(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
signals:
    
};



class VCard : public QObject
{
    Q_OBJECT
public:
    explicit VCard(DbAdapter *db, QObject *parent = nullptr);
    
    void exportVCF(QString filepath);

private:
    DbAdapter *db;
    
signals:
    
};

#endif // VCARD_H
