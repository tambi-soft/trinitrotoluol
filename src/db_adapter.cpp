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

QMap<QString,QVariant> DbAdapter::dbIteratorToMap(QSqlQuery query)
{
    QMap<QString,QVariant> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        //result[ query.value(0).toString() ] = query.value(1).toString();
        for (int i=0; i <= rec.count(); ++i)
        {
            QString name = rec.fieldName(i);
            QVariant value = query.value(i);
            
            result[name] = value;
        }
    }
    
    return result;
}

void DbAdapter::initializeTables()
{
    QSqlQuery query_table_people("CREATE TABLE IF NOT EXISTS \"people\" (\
        \"rowid\"	INTEGER PRIMARY KEY AUTOINCREMENT,\
        \"tnt_id\"    INTEGER,\
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
        \"notes\"	TEXT,\
        \"donations_monthly\"   INTEGER,\
        \"donations_monthly_promised\"  INTEGER,\
        \"spouse_rowid\"    INTEGER,\
        \"deactivated\"     INTEGER\
    )", this->db);
    
    QSqlQuery query_view_groups("CREATE VIEW IF NOT EXISTS groups AS\
        SELECT \"group\" FROM people GROUP BY \"group\";", this->db);
    
    QSqlQuery query_sent_mail("CREATE TABLE IF NOT EXISTS \"mail_sent\" (\
        \"people_rowid\"    INTEGER,\
        \"mail_id\"         INTEGER,\
        \"mail_number\"     INTEGER,\
        \"date\"            INTEGER\
    )", this->db);
    
    QSqlQuery query_donations("CREATE VIEW IF NOT EXISTS donations_monthly AS\
        SELECT SUM(donations_monthly) AS monthly_sum, SUM(donations_monthly_promised) AS monthly_sum_promised FROM people\
    ", this->db);
    
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

QMap<QString,QVariant> DbAdapter::selectPerson(qlonglong id)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT b.\"name\" AS \"spouse_name\", a.tnt_id, a.name, a.\"group\", a.\"email\", a.\"address\", a.\"phone\", a.\"agreed_mail\", a.\"agreed_prayer\", a.\"agreement\", a.\"notes\", a.\"donations_monthly\", a.\"donations_monthly_promised\"\
        FROM people a\
        LEFT JOIN people b ON a.spouse_rowid=b.rowid\
        WHERE a.rowid=:id");
    query.bindValue(":id", id);
    query.exec();
    
    return dbIteratorToMap(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectAllPersons()
{
    QSqlQuery query("SELECT rowid, \"name\", \"group\", \"email\", \"agreed_mail\", \"agreed_prayer\", \"agreement\" FROM people", this->db);
    
    return dbIteratorToMapList(query);
}
QList<QMap<QString,QVariant>> DbAdapter::selectAllPersonsFiltered(QString group, QString name, QString mail)
{
    QSqlQuery query(this->db);
    // the ORs should really be XORs, but SQLite do not support XOR now, and it would be far to annoying to fiddle an XOR together by myself
    query.prepare("SELECT rowid, \"name\", \"group\", \"email\", \"agreed_mail\", \"agreed_prayer\", \"agreement\" FROM people WHERE \"group\" LIKE :group AND (\"name\" LIKE :name OR \"name\" IS NULL) AND (email LIKE :mail OR email IS NULL)");
    query.bindValue(":group", group);
    query.bindValue(":name", name);
    query.bindValue(":mail", mail);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectGroups()
{
    QSqlQuery query("SELECT \"group\" FROM \"groups\"", this->db);
    
    return dbIteratorToMapList(query);
}
