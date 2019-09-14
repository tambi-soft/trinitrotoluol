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
    QSqlQuery bindPersonParams(QSqlQuery query, QMap<QString, QVariant> data);
    qlonglong insertNewPerson(QMap<QString, QVariant> data);
    void updatePerson(qlonglong rowid, QMap<QString,QVariant> data);
    void deletePerson(qlonglong rowid);
    QMap<QString, QVariant> selectPerson(qlonglong rowid);
    QList<QMap<QString, QVariant>> selectAllPersons();
    QList<QMap<QString, QVariant>> selectAllPersonsFiltered(QString group, QString name, QString mail);
    QList<QMap<QString, QVariant>> selectGroups();
    QMap<QString, QVariant> selectMoneyStats();
    QMap<QString, QVariant> selectPeopleStats();
    
private:
    QSqlDatabase db;
    
    QList<QMap<QString,QVariant>> dbIteratorToMapList(QSqlQuery query);
    QMap<QString, QVariant> dbIteratorToMap(QSqlQuery query);
    
    void initializeTables();
    
signals:
    
public slots:
    
};

#endif // DB_ADAPTER_H
