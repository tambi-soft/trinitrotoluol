#include "db_merge.h"

DbMerge::DbMerge(Config *config, DbAccess *parent) : DbAccess(parent)
{
    this->config = config;
    
    QString db_name = "db_to_be_merged";
    
    if (! QSqlDatabase::contains(db_name))
    {
        QSqlDatabase::addDatabase("QSQLITE", db_name);
    }
    
    this->db = QSqlDatabase::database(db_name);
    
    QString path = config->getDbPath();
    this->db.setDatabaseName(path);
    
    if (! this->db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

QList<QMap<QString,QVariant>> DbMerge::selectTables()
{
    QSqlQuery query("SELECT name FROM sqlite_master WHERE type ='table' AND name NOT LIKE 'sqlite_%';", this->db);
    
    return dbIteratorToMapList(query);
}
