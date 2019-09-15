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
        \"group_rowid\"	INTEGER,\
        \"tnt_id\"    INTEGER,\
        \"name\"	TEXT,\
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
        \"deactivated\"     INTEGER,\
        \"flag_todo\"       INTEGER,\
        \"flag_waiting\"    INTEGER\
    )", this->db);
    
    //QSqlQuery query_view_groups("CREATE VIEW IF NOT EXISTS groups AS\
        SELECT \"group\" FROM people GROUP BY \"group\";", this->db);
    QSqlQuery query_groups("CREATE TABLE IF NOT EXISTS \"groups\" (rowid INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)", this->db);
    
    QSqlQuery query_sent_mail("CREATE TABLE IF NOT EXISTS \"mail_sent\" (\
        \"rowid_people\"    INTEGER,\
        \"rowid_mail\"         INTEGER,\
        \"date\"            INTEGER\
    )", this->db);
            
    QSqlQuery query_mail("CREATE TABLE IF NOT EXISTS \"mail\" ("
                         "rowid INTEGER PRIMARY KEY AUTOINCREMENT,"
                         "number INTEGER,"
                         "subject TEXT,"
                         "cover TEXT,"
                         "content TEXT)", this->db);
    
    QSqlQuery query_donations("CREATE VIEW IF NOT EXISTS donations_monthly AS\
        SELECT SUM(donations_monthly) AS monthly_sum, SUM(donations_monthly_promised) AS monthly_sum_promised FROM people\
    ", this->db);
    
    QSqlQuery query_people_stats("CREATE VIEW IF NOT EXISTS people_stats AS "
        "SELECT COUNT(name) AS sum_all,"
        " SUM(agreed_mail) AS sum_agreed_mail,"
        "SUM(agreed_prayer) AS sum_agreed_prayer,"
        "SUM(CASE WHEN donations_monthly > 0 THEN 1 ELSE 0 END) AS donation_partners,"
        "SUM(CASE WHEN donations_monthly_promised > 0 THEN 1 ELSE 0 END) AS donation_partners_promised "
        "FROM people", this->db);
    
    //qDebug() << this->db.lastError();
    //qDebug() << query_sent_mail.lastQuery();
}

void DbAdapter::deletePerson(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

QSqlQuery DbAdapter::bindPersonParams(QSqlQuery query, QMap<QString,QVariant> data)
{
    query.bindValue(":tnt_id", data["tnt_id"].toInt());
    query.bindValue(":name", data["name"].toString());
    query.bindValue(":group", data["group"].toInt());
    query.bindValue(":email", data["email"].toString());
    query.bindValue(":address", data["address"].toString());
    query.bindValue(":phone", data["phone"].toString());
    query.bindValue(":agreed_mail", data["agreed_mail"].toInt());
    query.bindValue(":agreed_prayer", data["agreed_prayer"].toInt());
    query.bindValue(":agreement", data["agreement"].toString());
    query.bindValue(":notes", data["notes"].toString());
    query.bindValue(":donations_monthly", data["donations_monthly"].toInt());
    query.bindValue(":donations_monthly_promised", data["donations_monthly_promised"].toInt());
    query.bindValue(":flag_todo", data["flag_todo"]);
    query.bindValue(":flag_waiting", data["flag_waiting"]);
    
    return query;
}

qlonglong DbAdapter::insertNewPerson(QMap<QString,QVariant> data)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people"
                  "(tnt_id, name, group_rowid, email, address, phone, agreed_mail, agreed_prayer, agreement, notes, donations_monthly, donations_monthly_promised, flag_todo, flag_waiting, date_collected)"
                  "VALUES"
                  "(:tnt_id, :name, :group, :email, :address, :phone, :agreed_mail, :agreed_prayer, :agreement, :notes, :donations_monthly, :donations_monthly_promised, :flag_todo, :flag_waiting, CURRENT_TIMESTAMP)");
    
    query = bindPersonParams(query, data);
    
    query.exec();
    
    //qDebug() << this->db.lastError();
    //qDebug() << query.lastQuery();
    
    this->db.commit();
    
    return query.lastInsertId().toLongLong();
}

void DbAdapter::updatePerson(qlonglong rowid, QMap<QString,QVariant> data)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people SET "
                  "tnt_id=:tnt_id, name=:name, group_rowid=:group, email=:email, address=:address, phone=:phone, agreed_mail=:agreed_mail, agreed_prayer=:agreed_prayer, agreement=:agreement, notes=:notes, donations_monthly=:donations_monthly, donations_monthly_promised=:donations_monthly_promised, flag_todo=:flag_todo, flag_waiting=:flag_waiting, date_last_changed=CURRENT_TIMESTAMP"
                  " WHERE rowid=:rowid");
    
    query = bindPersonParams(query, data);
    
    query.bindValue(":rowid", rowid);
    query.exec();
}

QMap<QString,QVariant> DbAdapter::selectPerson(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT b.name AS spouse_name, a.tnt_id, a.name, a.group_rowid, g.name AS group_name, a.\"email\", a.\"address\", a.\"phone\", a.\"agreed_mail\", a.\"agreed_prayer\", a.\"agreement\", a.\"notes\", a.\"donations_monthly\", a.\"donations_monthly_promised\", a.flag_todo, a.flag_waiting\
        FROM people a\
        LEFT JOIN people b ON a.spouse_rowid=b.rowid\
        JOIN groups g ON a.group_rowid=g.rowid\
        WHERE a.rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

/*
QList<QMap<QString,QVariant>> DbAdapter::selectAllPersons()
{
    QSqlQuery query("SELECT rowid, \"name\", \"group\", email, agreed_mail, agreed_prayer, agreement, flag_todo, flag_waiting FROM people", this->db);
    
    return dbIteratorToMapList(query);
}
*/
QList<QMap<QString,QVariant>> DbAdapter::selectAllPersonsFiltered(QString todo, QString waiting, QString group, QString name, QString mail)
{
    QSqlQuery query(this->db);
    // the ORs should really be XORs, but SQLite do not support XOR now, and it would be far to annoying to fiddle a XOR together by myself
    query.prepare("SELECT people.rowid, people.name, groups.name AS \"group\", email, agreed_mail, agreed_prayer, agreement, flag_todo, flag_waiting, donations_monthly, donations_monthly_promised\
                  FROM people\
                  JOIN groups ON people.group_rowid=groups.rowid\
                  WHERE groups.name LIKE :group\
                  AND (people.name LIKE :name OR people.name IS NULL)\
                  AND (email LIKE :mail OR email IS NULL)");
    query.bindValue(":group", group);
    query.bindValue(":name", name);
    query.bindValue(":mail", mail);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectGroups()
{
    QSqlQuery query("SELECT rowid, name FROM groups", this->db);
    
    return dbIteratorToMapList(query);
}

QMap<QString,QVariant> DbAdapter::selectMoneyStats()
{
    QSqlQuery query("SELECT monthly_sum, monthly_sum_promised FROM donations_monthly", this->db);
    
    return dbIteratorToMap(query);
}

QMap<QString,QVariant> DbAdapter::selectPeopleStats()
{
    QSqlQuery query("SELECT sum_all, sum_agreed_mail, sum_agreed_prayer, donation_partners, donation_partners_promised FROM people_stats", this->db);
    
    return  dbIteratorToMap(query);
}
