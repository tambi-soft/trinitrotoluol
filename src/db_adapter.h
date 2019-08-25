#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDir>
#include <QDateTime>

#include "config.h"

class DbAdapter : public QObject
{
    Q_OBJECT
    
public:
    explicit DbAdapter(Config *config, QObject *parent = nullptr);
    void insertNewPerson(QString name, QString group, QString email, QString address, QString phone);
    QList<QMap<QString, QVariant>> selectPerson(qlonglong id);
    QList<QMap<QString, QVariant> > selectAllPerons();
    
private:
    QSqlDatabase db;
    
    QList<QMap<QString,QVariant>> dbIteratorToMapList(QSqlQuery query);
    QMap<QString,QString> dbIteratorToMap(QSqlQuery query);
    
    void initializeTables();
    
signals:
    
public slots:
};

#endif // DB_ADAPTER_H
