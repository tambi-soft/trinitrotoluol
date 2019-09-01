#include "sqlite_sync.h"

SQLiteSync::SQLiteSync(QString path_current_db, QString path_conflicting_db)
{
    // this has to be the same as "db_name" in class DbAdapter
    QString name_db_current = "trinitro";
    
    QString name_db_conflicting = "trinitro_conflicting";
    
    if (! QSqlDatabase::contains(name_db_current))
    {
        QSqlDatabase::addDatabase("QSQLITE", name_db_current);
    }
    
    if (! QSqlDatabase::contains(name_db_conflicting))
    {
        QSqlDatabase::addDatabase("QSQLITE", name_db_conflicting);
    }
    
    this->db_current = QSqlDatabase::database(name_db_current);
    this->db_conflicting = QSqlDatabase::database(name_db_conflicting);
    
    this->db_current.setDatabaseName(path_current_db);
    this->db_conflicting.setDatabaseName(path_conflicting_db);
    
    if (! this->db_current.open())
    {
        qDebug() << "Error: db current connection fail";
    }
    
    if (! this->db_conflicting.open())
    {
        qDebug() << "Error: db conflicting connection fail";
    }
}
