#ifndef SQLITE_SYNC_H
#define SQLITE_SYNC_H

#include <QObject>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class SQLiteSync
{
public:
    SQLiteSync(QString path_current_db, QString path_conflicting_db);
    
private:
    QSqlDatabase db_current;
    QSqlDatabase db_conflicting;
};

#endif // SQLITE_SYNC_H
