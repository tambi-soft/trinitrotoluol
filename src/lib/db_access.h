#ifndef DbAccess_H
#define DbAccess_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMap>
#include <QVariant>

class DbAccess : public QObject
{
    Q_OBJECT
    
public:
    explicit DbAccess(QObject *parent = nullptr);
    
    QList<QMap<QString,QVariant>> dbIteratorToMapList(QSqlQuery query);
    QMap<QString, QVariant> dbIteratorToMap(QSqlQuery query);
    
signals:
    
};

#endif // DbAccess_H
