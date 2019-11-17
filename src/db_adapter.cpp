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
    QSqlQuery query_table_people("CREATE TABLE IF NOT EXISTS people ( "
        "rowid	INTEGER PRIMARY KEY AUTOINCREMENT, "
        "group_rowid	INTEGER, "
        "tnt_id    INTEGER, "
        "name	TEXT, "
        "email	TEXT, "
        "address	TEXT, "
        "phone	TEXT, "
        "agreed_mail	INTEGER DEFAULT 0, "
        "agreed_prayer	INTEGER DEFAULT 0, "
        "agreement	TEXT, "
        "date_collected  INTEGER, "
        "date_last_changed INTEGER, "
        "notes	TEXT, "
        "donations_monthly INTEGER DEFAULT 0, "
        "donations_monthly_promised  INTEGER DEFAULT 0, "
        "spouse_rowid    INTEGER, "
        "flag_deactivated     INTEGER DEFAULT 0, "
        "flag_todo       INTEGER DEFAULT 0, "
        "flag_waiting    INTEGER DEFAULT 0, "
        "flag_supporter  INTEGER DEFAULT 0)", this->db);
    
    QSqlQuery query_groups("CREATE TABLE IF NOT EXISTS people_groups (rowid INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)", this->db);
    
    QSqlQuery query_people_visited("CREATE TABLE IF NOT EXISTS people_visits ( "
        "rowid INTEGER PRIMARY KEY AUTOINCREMENT "
        "rowid_people INTEGER, "
        "rowid_journeys INTEGER, "
        "date TEXT, "
        "notes TEXT)", this->db);
    
    QSqlQuery query_journeys("CREATE TABLE IF NOT EXISTS journeys ( "
        "rowid INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, "
        "date_from TEXT, "
        "date_to TEXT, "
        "notes TEXT)", this->db);
    
    QSqlQuery query_tickets("CREATE TABLE IF NOT EXISTS journey_tickets ( "
        "rowid INTEGER PRIMARY KEY AUTOINCREMENT, "
        "rowid_journeys INTEGER, "
        "name TEXT, "
        "cost INTEGER, "
        "flag_settled INTEGER, "
        "notes TEXT)", this->db);
            
    QSqlQuery query_mail("CREATE TABLE IF NOT EXISTS mail ( "
        "rowid INTEGER PRIMARY KEY AUTOINCREMENT, "
        "number TEXT, "
        "subject TEXT, "
        "alternative_text TEXT, "
        "cover TEXT, "
        "content_path TEXT, "
        "attachment_path_one TEXT, "
        "attachment_path_two TEXT, "
        "date TEXT, "
        "date_last_edit TEXT)", this->db);
    
    QSqlQuery query_mail_attachment("CREATE TABLE IF NOT EXISTS mail_attachments ( "
        "rowid_mail INTEGER PRIMARY KEY, "
        "path TEXT)", this->db);
    
    QSqlQuery query_sent_mail("CREATE TABLE IF NOT EXISTS mail_sent (\
        \"rowid_mail\"      INTEGER,\
        \"rowid_people\"    INTEGER,\
        \"date\"            INTEGER)", this->db);
    
    QSqlQuery query_donations("CREATE VIEW IF NOT EXISTS donations_monthly AS "
        "SELECT SUM(donations_monthly) AS monthly_sum, "
        "SUM(donations_monthly_promised) AS monthly_sum_promised, "
        "MAX(MAX(donations_monthly), MAX(donations_monthly_promised)) AS donations_max, "
        "MIN(MIN(NULLIF(donations_monthly,0)),  MIN(NULLIF(donations_monthly_promised,0))) AS donations_min, "
        "(SUM(donations_monthly) + SUM(donations_monthly_promised)) "
         " / (COUNT(NULLIF(donations_monthly,0)) + COUNT(NULLIF(donations_monthly_promised,0))) AS donations_average "
        "FROM people", this->db);
    
    QSqlQuery query_people_stats("CREATE VIEW IF NOT EXISTS people_stats AS "
        "SELECT COUNT(name) AS sum_all,"
        " SUM(agreed_mail) AS sum_agreed_mail,"
        "SUM(agreed_prayer) AS sum_agreed_prayer,"
        "SUM(CASE WHEN donations_monthly > 0 THEN 1 ELSE 0 END) AS donation_partners,"
        "SUM(CASE WHEN donations_monthly_promised > 0 THEN 1 ELSE 0 END) AS donation_partners_promised "
        "FROM people "
        "WHERE flag_deactivated < 1", this->db);
    
    QSqlQuery query_settings("CREATE TABLE IF NOT EXISTS settings (key TEXT PRIMARY KEY, value TEXT)", this->db);
    
    QSqlQuery query_currencies("CREATE TABLE IF NOT EXISTS currencies (rowid INTEGER PRIMARY KEY AUTOINCREMENT, code TEXT, exchange_rate INTEGER, flag_default INTEGER UNIQUE, notes TEXT)", this->db);
    
    QSqlQuery query_people_donations("CREATE TABLE IF NOT EXISTS people_donations (rowid_people INTEGER PRIMARY KEY, amount INTEGER, date TEXT)", this->db);
    
    QSqlQuery query_expenses("CREATE TABLE IF NOT EXISTS expenses (rowid INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, amount INTEGER, cost_one INTEGER, rowid_currency INTEGER, date TEXT DEFAULT CURRENT_TIMESTAMP, notes TEXT, flag_settled INTEGER)", this->db);
    
    //qDebug() << this->db.lastError();
    //qDebug() << query_sent_mail.lastQuery();
}

void DbAdapter::commit()
{
    this->db.commit();
}

void DbAdapter::deletePerson(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}
void DbAdapter::deactivatePerson(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people SET flag_deactivated=1 WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

void DbAdapter::linkSpouses(qlonglong rowid_a, qlonglong rowid_b)
{
    QSqlQuery query_a(this->db);
    query_a.prepare("UPDATE people SET spouse_rowid=:rowid_a WHERE rowid=:rowid_b");
    query_a.bindValue(":rowid_a", rowid_a);
    query_a.bindValue(":rowid_b", rowid_b);
    query_a.exec();
    
    QSqlQuery query_b(this->db);
    query_b.prepare("UPDATE people SET spouse_rowid=:rowid_b WHERE rowid=:rowid_a");
    query_b.bindValue(":rowid_a", rowid_a);
    query_b.bindValue(":rowid_b", rowid_b);
    query_b.exec();
}

void DbAdapter::unlinkSpouses(qlonglong rowid_a, qlonglong rowid_b)
{
    QSqlQuery query_a(this->db);
    query_a.prepare("UPDATE people SET spouse_rowid=NULL WHERE spouse_rowid=:rowid");
    query_a.bindValue(":rowid", rowid_a);
    query_a.exec();
    
    QSqlQuery query_b(this->db);
    query_b.prepare("UPDATE people SET spouse_rowid=NULL WHERE spouse_rowid=:rowid");
    query_a.bindValue(":rowid", rowid_b);
    query_b.exec();
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

qlonglong DbAdapter::insertNewPerson()
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people (date_collected) VALUES (CURRENT_TIMESTAMP)");
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}
/*
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
*/
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
    query.prepare("SELECT b.name AS spouse_name, a.tnt_id, a.name, a.group_rowid, g.name AS group_name, a.email, a.address, a.phone, a.agreed_mail, a.agreed_prayer, a.agreement, a.notes, a.donations_monthly, a.donations_monthly_promised, a.flag_todo, a.flag_waiting "
        "FROM people a "
        "LEFT JOIN people b ON a.spouse_rowid=b.rowid "
        "LEFT JOIN people_groups g ON a.group_rowid=g.rowid "
        "WHERE a.rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectAllPersonsFiltered(int todo, int waiting, int donating, int deactivated, int agreed_mail, QString group, QString name, QString mail)
{
    QSqlQuery query(this->db);
    // the ORs in the last two lines should really be XORs, but SQLite do not support XOR for now, and it would be far to annoying to fiddle a XOR together by myself
    query.prepare("SELECT people.rowid, people.name, people_groups.name AS \"group\", email, agreed_mail, agreed_prayer, agreement, flag_todo, flag_waiting, donations_monthly, donations_monthly_promised "
                  "FROM people "
                  "LEFT JOIN people_groups ON people.group_rowid=people_groups.rowid "
                  "WHERE "
                  "CASE "
                      "WHEN (:todo=0) THEN flag_todo = 0 OR flag_todo IS NULL "
                      "WHEN (:todo=1) THEN flag_todo = 1 "
                      "ELSE (flag_todo = flag_todo) OR flag_todo IS NULL "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:waiting=0) THEN flag_waiting = 0 OR flag_waiting IS NULL "
                      "WHEN (:waiting=1) THEN flag_waiting = 1 "
                      "ELSE (flag_waiting = flag_waiting) OR flag_waiting IS NULL "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:donating=0) THEN (donations_monthly = 0 AND donations_monthly_promised = 0) "
                      "WHEN (:donating=1) THEN (donations_monthly > 0 OR donations_monthly_promised > 0) "
                      "ELSE (donations_monthly = donations_monthly) "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:deactivated=0) THEN flag_deactivated = 0 OR flag_deactivated IS NULL "
                      "WHEN (:deactivated=1) THEN flag_deactivated = 1 "
                      "ELSE (flag_deactivated = flag_deactivated) OR flag_deactivated IS NULL "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:agreed_mail=0) THEN agreed_mail = 0 "
                      "WHEN (:agreed_mail=1) THEN agreed_mail = 1 "
                      "ELSE agreed_mail = agreed_mail "
                  "END "
                  "AND (people_groups.name LIKE :group OR people_groups.name IS NULL) "
                  "AND (people.name LIKE :name OR people.name IS NULL) "
                  "AND (email LIKE :mail OR email IS NULL) ");
    query.bindValue(":todo", todo);
    query.bindValue(":waiting", waiting);
    query.bindValue(":donating", donating);
    query.bindValue(":deactivated", deactivated);
    query.bindValue(":agreed_mail", agreed_mail);
    query.bindValue(":group", group);
    query.bindValue(":name", name);
    query.bindValue(":mail", mail);
    query.exec();
    
    //qDebug() << this->db.lastError();
    //qDebug() << query.lastQuery();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectAllPersonsForMail(bool agreed_mail)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT name, email FROM people "
                  "WHERE flag_deactivated = 0 "
                  "AND agreed_mail=:agreed_mail");
    query.bindValue(":agreed_mail", agreed_mail);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectVisitsForPerson(qlonglong rowid_person)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people_visits.rowid, rowid_journeys, people_visits.date, people_visits.notes, journeys.name AS journey_name "
                  "FROM people_visits "
                  "JOIN journeys ON journeys.rowid=rowid_journeys "
                  "WHERE rowid_people=:rowid_people "
                  "ORDER BY people_visits.date DESC");
    query.bindValue(":rowid_people", rowid_person);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectMailsForPerson(qlonglong rowid_person)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT mail_sent.date, mail.number "
                  "FROM mail_sent "
                  "JOIN mail ON mail.rowid=rowid_mail "
                  "WHERE mail_sent.rowid_people=:rowid_people");
    query.bindValue(":rowid_people", rowid_person);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectGroups()
{
    QSqlQuery query("SELECT rowid, name FROM people_groups", this->db);
    
    return dbIteratorToMapList(query);
}

QMap<QString,QVariant> DbAdapter::selectMoneyStats()
{
    QSqlQuery query("SELECT monthly_sum, monthly_sum_promised, donations_min, donations_max, donations_average FROM donations_monthly", this->db);
    
    return dbIteratorToMap(query);
}

QMap<QString,QVariant> DbAdapter::selectPeopleStats()
{
    QSqlQuery query("SELECT sum_all, sum_agreed_mail, sum_agreed_prayer, donation_partners, donation_partners_promised FROM people_stats", this->db);
    
    return  dbIteratorToMap(query);
}

qlonglong DbAdapter::insertNewMail()
{
    QSqlQuery query("INSERT INTO mail (date) VALUES (CURRENT_TIMESTAMP)", this->db);
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::updateMail(qlonglong rowid, QMap<QString,QVariant> data)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE mail SET number=:number, subject=:subject, alternative_text=:alternative_text, cover=:cover, content_path=:content_path, attachment_path_one=:attachment_path_one, attachment_path_two=:attachment_path_two, date_last_edit=CURRENT_TIMESTAMP WHERE rowid=:rowid");
    query.bindValue(":number", data["number"].toString());
    query.bindValue(":subject", data["subject"].toString());
    query.bindValue(":alternative_text", data["alternative_text"].toString());
    query.bindValue(":cover", data["cover"].toString());
    query.bindValue(":content_path", data["content_path"].toString());
    query.bindValue(":attachment_path_one", data["attachment_path_one"].toString());
    query.bindValue(":attachment_path_two", data["attachment_path_two"].toString());
    query.bindValue(":rowid", rowid);
    query.exec();
    
    qDebug() << data["alternative_text"].toString();
}

void DbAdapter::deleteMail(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM mail WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

QMap<QString,QVariant> DbAdapter::selectMail(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT number, subject, alternative_text, cover, content_path, attachment_path_one, attachment_path_two, date, date_last_edit "
                  "FROM mail "
                  "WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectAllMails()
{
    QSqlQuery query("SELECT rowid, number, subject, cover, content_path, date, date_last_edit FROM mail ORDER BY number", this->db);
    
    return dbIteratorToMapList(query);
}

void DbAdapter::insertMailSent(qlonglong rowid_mail, qlonglong rowid_people)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO mail_sent (rowid_people, rowid_mail, date) VALUES (:rowid_people, :rowid_mail, CURRENT_TIMESTAMP)");
    query.bindValue(":rowid_mail", rowid_mail);
    query.bindValue(":rowid_people", rowid_people);
    query.exec();
}

void DbAdapter::insertSettings(QString key, QString value)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:key, :value)");
    query.bindValue(":key", key);
    query.bindValue(":value", value);
    query.exec();
}

void DbAdapter::insertSettings(QString key, int value)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:key, :value)");
    query.bindValue(":key", key);
    query.bindValue(":value", value);
    query.exec();
}

QString DbAdapter::selectSettings(QString key)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT value FROM settings WHERE key=:key");
    query.bindValue(":key", key);
    query.exec();
    query.first();
    
    return query.value("value").toString();
}

QList<QMap<QString,QVariant>> DbAdapter::selectJourneys()
{
    QSqlQuery query("SELECT j.rowid, j.name, j.date_from, j.date_to, printf('%.2f', t.costs) AS costs "
                    "FROM journeys j "
                    "LEFT JOIN ( "
                        "SELECT rowid_journeys, SUM(cost) AS costs "
                        "FROM journey_tickets "
                        "GROUP BY rowid_journeys "
                    ") t ON t.rowid_journeys = j.rowid "
                    "ORDER BY date_from, date_to", this->db);
    
    return dbIteratorToMapList(query);
}

qlonglong DbAdapter::insertJourney(QString name, QString date_from, QString date_to, QString notes)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO journeys (name, date_from, date_to, notes) VALUES (:name, :date_from, :date_to, :notes)");
    query.bindValue(":name", name);
    query.bindValue(":date_from", date_from);
    query.bindValue(":date_to", date_to);
    query.bindValue(":notes", notes);
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::updateJourney(qlonglong rowid, QString name, QString date_from, QString date_to, QString notes)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE journeys SET name=:name, date_from=:date_from, date_to=:date_to, notes=:notes WHERE rowid=:rowid");
    query.bindValue(":name", name);
    query.bindValue(":date_from", date_from);
    query.bindValue(":date_to", date_to);
    query.bindValue(":notes", notes);
    query.bindValue(":rowid", rowid);
    query.exec();
}

QMap<QString,QVariant> DbAdapter::selectJourney(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid, name, date_from, date_to, notes FROM journeys WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectVisitsForJourney(qlonglong rowid_journey)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people_visits.rowid, rowid_people, people.name, date, people_visits.notes "
                  "FROM people_visits "
                  "LEFT JOIN people ON rowid_people=people.rowid "
                  "WHERE rowid_journeys=:rowid");
    query.bindValue(":rowid", rowid_journey);
    query.exec();
    
    return dbIteratorToMapList(query);
}

qlonglong DbAdapter::insertVisit(qlonglong rowid_journey)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_visits (rowid_journeys) VALUES (:rowid)");
    query.bindValue(":rowid", rowid_journey);
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::updateVisit(qlonglong rowid, qlonglong rowid_people, QString date, QString notes)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people_visits SET rowid_people=:rowid_people, date=:date, notes=:notes WHERE rowid=:rowid");
    query.bindValue(":rowid_people", rowid_people);
    query.bindValue(":date", date);
    query.bindValue(":notes", notes);
    query.bindValue(":rowid", rowid);
    query.exec();
}

QMap<QString,QVariant> DbAdapter::selectVisit(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid_people, people.name, people_visits.date, people_visits.notes "
                  "FROM people_visits "
                  "JOIN people ON rowid_people=people.rowid "
                  "WHERE people_visits.rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

void DbAdapter::deleteVisit(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_visits WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

QList<QMap<QString,QVariant>> DbAdapter::selectTicketsForJourney(qlonglong rowid_journey)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT journey_tickets.rowid, name, printf('%.2f', cost) AS cost, currencies.code AS currency_code, flag_settled, journey_tickets.notes "
                  "FROM journey_tickets "
                  "LEFT JOIN currencies ON rowid_currency=currencies.rowid "
                  "WHERE rowid_journeys=:rowid_journeys");
    query.bindValue(":rowid_journeys", rowid_journey);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QMap<QString,QVariant> DbAdapter::selectTicket(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT journey_tickets.name, cost, currencies.code AS currency_code, flag_settled, journey_tickets.notes "
                  "FROM journey_tickets "
                  "LEFT JOIN currencies ON rowid_currency=currencies.rowid "
                  "WHERE journey_tickets.rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

qlonglong DbAdapter::insertTicket(qlonglong rowid_journey)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO journey_tickets (rowid_journeys) VALUES (:rowid)");
    query.bindValue(":rowid", rowid_journey);
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::updateTicket(qlonglong rowid, QString name, double cost, qlonglong rowid_currency, bool flag_settled, QString notes)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE journey_tickets SET name=:name, cost=:cost, rowid_currency=:rowid_currency, flag_settled=:flag_settled, notes=:notes WHERE rowid=:rowid");
    query.bindValue(":name", name);
    query.bindValue(":cost", cost);
    query.bindValue(":rowid_currency", rowid_currency);
    query.bindValue(":flag_settled", flag_settled);
    query.bindValue(":notes", notes);
    query.bindValue(":rowid", rowid);
    query.exec();
}

void DbAdapter::deleteTicket(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM journey_tickets WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

QList<QMap<QString,QVariant>> DbAdapter::selectExpenses()
{
    QSqlQuery query(this->db);
    query.prepare("SELECT expenses.rowid, name, amount, cost_one, currencies.code AS currency_code, date, expenses.notes, flag_settled "
                  "FROM expenses "
                  "LEFT JOIN currencies ON rowid_currency=currencies.rowid");
    query.exec();
    
    return dbIteratorToMapList(query);
}

QMap<QString,QVariant> DbAdapter::selectExpense(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT name, amount, cost_one, rowid_currency, date, notes, flag_settled FROM expenses WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

qlonglong DbAdapter::insertExpense()
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO expenses DEFAULT VALUES");
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::updateExpense(qlonglong rowid, qlonglong rowid_currency, QMap<QString,QVariant> data)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE expenses SET name=:name, amount=:amount, cost_one=:cost_one, rowid_currency=:rowid_currency, date=:date, notes=:notes, flag_settled=:flag_settled WHERE rowid=:rowid");
    query.bindValue(":name", data["name"].toString());
    query.bindValue(":amount", data["amount"].toDouble());
    query.bindValue(":cost_one", data["cost_one"].toDouble());
    query.bindValue(":date", data["date"].toString());
    query.bindValue(":notes", data["notes"].toString());
    query.bindValue(":flag_settled", data["flag_settled"].toInt());
    query.bindValue(":rowid", rowid);
    query.bindValue(":rowid_currency", rowid_currency);
    query.exec();
}

void DbAdapter::deleteExpense(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM expenses WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

QList<QMap<QString,QVariant>> DbAdapter::selectCurrencies()
{
    QSqlQuery query("SELECT rowid, code, exchange_rate, notes FROM currencies", this->db);
    
    return dbIteratorToMapList(query);
}
