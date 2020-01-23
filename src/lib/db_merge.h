#ifndef DBMERGE_H
#define DBMERGE_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "../config.h"
#include "db_access.h"

class DbMerge : public DbAccess
{
    Q_OBJECT
    
public:
    DbMerge(Config *config, DbAccess *parent = nullptr);
    Config *config;
    
    QList<QMap<QString,QVariant>> selectTables();
    QList<QMap<QString,QVariant>> selectColumns(QString table);
    
private:
    QSqlDatabase db;
};

#endif // DBMERGE_H
