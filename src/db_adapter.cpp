#include "db_adapter.h"

DbAdapter::DbAdapter(Config *config, QObject *parent) : QObject(parent)
{
    QString db_name = "trinitro";
    
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
    
    initializeTables();
}

// maps the selected field name to the fetched value
QList<QMap<QString,QVariant>> DbAdapter::dbIteratorToMapList(QSqlQuery query)
{
    QList<QMap<QString,QVariant>> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        QMap<QString,QVariant> map;
        for (int i = 0; i <= rec.count(); ++i)
        {
            QString name = rec.fieldName(i);
            QString value = query.value(i).toString();
            
            map[name] = value;
        }
        
        result.append(map);
    }
    
    return result;
}

QMap<QString,QString> DbAdapter::dbIteratorToMap(QSqlQuery query)
{
    QMap<QString,QString> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        result[ query.value(0).toString() ] = query.value(1).toString();
    }
    
    return result;
}

void DbAdapter::initializeTables()
{
    QSqlQuery query_table_people("CREATE TABLE IF NOT EXISTS \"people\" (\
        \"rowid\"	INTEGER PRIMARY KEY AUTOINCREMENT,\
        \"name\"	TEXT,\
        \"group\"	TEXT,\
        \"email\"	TEXT,\
        \"address\"	TEXT,\
        \"phone\"	TEXT,\
        \"agreed_mail\"	INTEGER,\
        \"agreed_prayer\"	INTEGER,\
        \"agreement\"	TEXT,\
        \"date_collected\"  INTEGER,\
        \"date_last_changed\" INTEGER,\
        \"notes\"	TEXT\
    )", this->db);
    
    QSqlQuery query_view_groups("CREATE VIEW IF NOT EXISTS groups AS\
        SELECT \"group\" FROM people GROUP BY \"group\";", this->db);
    
    QSqlQuery query_sent_mail("CREATE TABLE IF NOT EXISTS \"mail_sent\" (\
        \"people_rowid\"    INTEGER,\
        \"mail_id\"         INTEGER,\
        \"mail_number\"     INTEGER,\
        \"date\"            INTEGER\
    )", this->db);
    
    //qDebug() << this->db.lastError();
    //qDebug() << query_sent_mail.lastQuery();
}

void DbAdapter::insertNewPerson(QString name, QString group, QString email, QString address, QString phone)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people (\"name\", \"group\", \"email\", \"address\", \"phone\") VALUES (:name, :group, :email, :address, :phone)");
    query.bindValue(":name", name);
    query.bindValue(":group", group);
    query.bindValue(":email", email);
    query.bindValue(":address", address);
    query.bindValue(":phone", phone);
    query.exec();
}

QMap<QString,QString> DbAdapter::selectPerson(qlonglong id)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT \"name\", \"group\", \"email\", \"address\", \"phone\" FROME people WHERE id=:id");
    query.bindValue(":id", id);
    
    return dbIteratorToMap(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectAllPersons()
{
    QSqlQuery query("SELECT \"name\", \"group\", \"email\", \"agreed_mail\", \"agreed_prayer\", \"agreement\" FROM people", this->db);
    
    return dbIteratorToMapList(query);
}
