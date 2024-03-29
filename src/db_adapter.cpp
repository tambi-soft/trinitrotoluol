#include "db_adapter.h"

DbAdapter::DbAdapter(Config *config, DbAccess *parent) : DbAccess(parent)
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

void DbAdapter::initializeTables()
{
    QSqlQuery query_table_people("CREATE TABLE IF NOT EXISTS people ( "
        "rowid	INTEGER PRIMARY KEY AUTOINCREMENT, "
        "donorid INTEGER UNIQE, "
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
        "donations_monthly_promised  INTEGER DEFAULT 0, "
        "flag_deactivated     INTEGER DEFAULT 0, "
        "flag_todo       INTEGER DEFAULT 0, "
        "flag_waiting    INTEGER DEFAULT 0, "
        "flag_supporter  INTEGER DEFAULT 0)", this->db);
    
    QSqlQuery query_people_vcard("CREATE TABLE IF NOT EXISTS people_vcard ( "
                                 "rowid INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "rowid_people INTEGER, "
                                 "key TEXT, "
                                 "value TEXT )", this->db);
    
    QSqlQuery query_groups("CREATE TABLE IF NOT EXISTS people_groups (rowid INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, color TEXT)", this->db);
    //QSqlQuery query_tags("CREATE TABLE IF NOT EXISTS people_tags (rowid INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)", this->db);
    QSqlQuery query_tags_people("CREATE TABLE IF NOT EXISTS people_groups_matrix (rowid_people INTEGER, rowid_groups INTEGER)", this->db);
    
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
        "MIN(NULLIF(donations_monthly,0)) AS donations_min, "
        "MIN(NULLIF(donations_monthly_promised,0)) AS donations_monthly_min, "
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
    
    QSqlQuery query_settings("CREATE TABLE IF NOT EXISTS settings ("
                             "key TEXT PRIMARY KEY,"
                             " value TEXT)", this->db);
    
    QSqlQuery query_currencies("CREATE TABLE IF NOT EXISTS currencies ("
                               "rowid INTEGER PRIMARY KEY AUTOINCREMENT,"
                               " code TEXT,"
                               " exchange_rate INTEGER,"
                               " flag_default INTEGER UNIQUE,"
                               " notes TEXT)", this->db);
    
    QSqlQuery query_people_donations("CREATE TABLE IF NOT EXISTS people_donations ("
                                     "rowid INTEGER PRIMARY KEY,"
                                     " rowid_people INTEGER,"
                                     " person_name TEXT,"
                                     " amount REAL,"
                                     " rowid_currencies INTEGER,"
                                     " date TEXT,"
                                     " memo TEXT,"
                                     " tnt_code TEXT,"
                                     " mandate_reference TEXT,"
                                     " bank_name TEXT)", this->db);
    
    QSqlQuery query_expenses("CREATE TABLE IF NOT EXISTS expenses ("
                             "rowid INTEGER PRIMARY KEY AUTOINCREMENT,"
                             " name TEXT,"
                             " amount INTEGER,"
                             " cost_one INTEGER,"
                             " rowid_currency INTEGER,"
                             " date TEXT DEFAULT CURRENT_TIMESTAMP,"
                             " notes TEXT,"
                             " flag_settled INTEGER)", this->db);
    
    QSqlQuery query_people_relatives("CREATE TABLE IF NOT EXISTS people_relations_matrix ("
                                     "rowid_people_a INTEGER KEY,"
                                     " rowid_people_b INTEGER KEY,"
                                     " rowid_people_relations)", this->db);
    QSqlQuery query_people_relatives_labels("CREATE TABLE IF NOT EXISTS people_relations ("
                                            "rowid INTEGER PRIMARY KEY AUTOINCREMENT,"
                                            " name TEXT,"
                                            " color TEXT)", this->db);
    
    QSqlQuery query_people_import_map("CREATE TABLE IF NOT EXISTS people_donations_map ("
                                      "tnt_name TEXT,"
                                      "rowid_people INTEGER)", this->db);
    
    QSqlQuery query_vcard_import("CREATE TABLE IF NOT EXISTS vcard_import_original ("
                                 "rowid INTEGER PRIMARY KEY AUTOINCREMENT, rowid_people INTEGER, content TEXT)", this->db);
    
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

QSqlQuery DbAdapter::bindPersonParams(QSqlQuery query, QMap<QString,QVariant> data)
{
    query.bindValue(":tnt_id", data["tnt_id"].toInt());
    query.bindValue(":name", data["name"].toString());
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

void DbAdapter::personInsertTNTID(qlonglong rowid_person, QString tnt_id)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people SET tnt_id=:tnt_id WHERE rowid=:rowid");
    query.bindValue(":tnt_id", tnt_id);
    query.bindValue(":rowid", rowid_person);
    query.exec();
    this->db.commit();
}

void DbAdapter::personUpdateDonationsMap(qlonglong rowid_people, QString tnt_name)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people_donations_map SET rowid_people=:rowid_people WHERE tnt_name=:tnt_name");
    query.bindValue(":rowid_people", rowid_people);
    query.bindValue(":tnt_name", tnt_name);
    query.exec();
}

void DbAdapter::personInsertDonationsMap(qlonglong rowid_people, QString tnt_name)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_donations_map (tnt_name, rowid_people) VALUES (:tnt_name, :rowid_people)");
    query.bindValue(":rowid_people", rowid_people);
    query.bindValue(":tnt_name", tnt_name);
    query.exec();
}

void DbAdapter::personDeleteDonationsMap(qlonglong rowid_people)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_donations_map WHERE rowid_people=:rowid_people");
    query.bindValue(":rowid_people", rowid_people);
    query.exec();
}

QMap<QString,QVariant> DbAdapter::personSelectDonationsMap(QString tnt_name)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid_people FROM people_donations_map WHERE tnt_name=:tnt_name LIMIT 1");
    query.bindValue(":tnt_name", tnt_name);
    query.exec();
    
    return dbIteratorToMap(query);
}

QList<QMap<QString,QVariant>> DbAdapter::personSelectDonationsMap()
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people_donations_map.rowid_people, people.name, tnt_name "
                  "FROM people_donations_map "
                  "JOIN people ON people_donations_map.rowid_people = people.rowid");
    query.exec();
    
    return dbIteratorToMapList(query);
}

void DbAdapter::updatePerson(qlonglong rowid, QMap<QString,QVariant> data)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people SET "
                  "tnt_id=:tnt_id, name=:name, email=:email, address=:address, phone=:phone, agreed_mail=:agreed_mail, agreed_prayer=:agreed_prayer, agreement=:agreement, notes=:notes, donations_monthly_promised=:donations_monthly_promised, flag_todo=:flag_todo, flag_waiting=:flag_waiting, date_last_changed=CURRENT_TIMESTAMP"
                  " WHERE rowid=:rowid");
    
    query = bindPersonParams(query, data);
    
    query.bindValue(":rowid", rowid);
    query.exec();
}

QMap<QString,QVariant> DbAdapter::selectPerson(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT a.tnt_id, a.name, a.email, a.address, a.phone, a.agreed_mail, a.agreed_prayer, a.agreement, a.notes, a.donations_monthly_promised, a.flag_todo, a.flag_waiting "
        "FROM people a "
        "WHERE a.rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

qlonglong DbAdapter::personRowidForTNTCode(QString code)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid FROM people WHERE tnt_id=:tnt_id");
    query.bindValue(":tnt_id", code.toInt());
    query.exec();
    
    return dbIteratorToMap(query)["rowid"].toLongLong();
}

qlonglong DbAdapter::personNewVCardEntry(qlonglong rowid_person, QString vcard_type)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_vcard (rowid_people, key) VALUES (:rowid_people, :key)");
    query.bindValue(":rowid_people", rowid_person);
    query.bindValue(":key", vcard_type);
    query.exec();

    this->db.commit();
    return query.lastInsertId().toLongLong();
}
void DbAdapter::personDeleteVCardEntry(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_vcard WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}
void DbAdapter::personUpdateVCardEntry(qlonglong rowid, QString value)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people_vcard SET value=:value WHERE rowid=:rowid");
    query.bindValue(":value", value);
    query.bindValue(":rowid", rowid);
    query.exec();
}
QList<QMap<QString,QVariant>> DbAdapter::personSelectVCardEntry(qlonglong rowid_person)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid, key, value FROM people_vcard WHERE rowid_people=:rowid_people");
    query.bindValue(":rowid_people", rowid_person);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::personSelectExport()
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people.rowid, name, email, address, phone, notes "
                  "FROM people "
                  "WHERE people.flag_deactivated != 1");
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::personSelectVCardExport(qlonglong rowid_person)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people_vcard.key, people_vcard.value "
                  "FROM people_vcard "
                  "WHERE people_vcard.rowid_people=:rowid_people");
    query.bindValue(":rowid_people", rowid_person);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectAllPersonsFiltered(int todo, int waiting, int donating, int deactivated, int agreed_mail, QString group, QString name, QString mail)
{
    QSqlQuery query(this->db);
    // the ORs in the last two lines should really be XORs, but SQLite do not support XOR for now, and it would be far to annoying to fiddle a XOR together by myself
    query.prepare("SELECT people.rowid, people.name, email, "
                  "GROUP_CONCAT(DISTINCT people_groups.name) AS groups_names, "
                  "GROUP_CONCAT(DISTINCT people_groups.color) AS groups_colors, "
                  "agreed_mail, agreed_prayer, agreement, flag_todo, flag_waiting, donations_monthly_promised, people_donations.amount AS donations_received "
                  "FROM people "
                  "LEFT JOIN people_donations ON people.rowid=people_donations.rowid_people "
                  "LEFT JOIN people_groups_matrix ON people_groups_matrix.rowid_people=people.rowid "
                  "LEFT JOIN people_groups ON people_groups_matrix.rowid_groups=people_groups.rowid "
                  "WHERE "
                  "CASE "
                      "WHEN (:todo=0) THEN flag_todo = 0 OR flag_todo IS NULL "
                      "WHEN (:todo=1) THEN flag_todo = 1 "
                      "ELSE true "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:waiting=0) THEN flag_waiting = 0 OR flag_waiting IS NULL "
                      "WHEN (:waiting=1) THEN flag_waiting = 1 "
                      "ELSE true "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:donating=0) THEN (people_donations.amount = 0 OR people_donations.amount IS NULL AND donations_monthly_promised = 0) "
                      "WHEN (:donating=1) THEN (people_donations.amount > 0 OR donations_monthly_promised > 0) "
                      "ELSE true "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:deactivated=0) THEN flag_deactivated = 0 OR flag_deactivated IS NULL "
                      "WHEN (:deactivated=1) THEN flag_deactivated = 1 "
                      "ELSE true "
                  "END "
                  "AND "
                  "CASE "
                      "WHEN (:agreed_mail=0) THEN agreed_mail = 0 "
                      "WHEN (:agreed_mail=1) THEN agreed_mail = 1 "
                      "ELSE true "
                  "END "
                  "AND (people_groups.name LIKE :group OR people_groups.name IS NULL) "
                  "AND (people.name LIKE :name OR people.name IS NULL) "
                  "AND (email LIKE :mail OR email IS NULL) "
                  "GROUP BY people.rowid");
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
                  "LEFT JOIN journeys ON journeys.rowid=rowid_journeys "
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
                  "WHERE mail_sent.rowid_people=:rowid_people "
                  "ORDER BY mail_sent.date DESC");
    query.bindValue(":rowid_people", rowid_person);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectGroups()
{
    QSqlQuery query("SELECT people_groups.rowid, people_groups.name, people_groups.color, COUNT(rowid_people) AS count"
                    " FROM people_groups"
                    " LEFT JOIN people_groups_matrix ON people_groups_matrix.rowid_groups=people_groups.rowid"
                    " GROUP BY people_groups.rowid", this->db);
    
    return dbIteratorToMapList(query);
}

qlonglong DbAdapter::insertNewGroup()
{
    QSqlQuery query("INSERT INTO people_groups DEFAULT VALUES", this->db);

    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::deleteGroup(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_groups WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    QSqlQuery quer(this->db);
    quer.prepare("DELETE FROM people_groups_matrix WHERE rowid_groups=:rowid");
    quer.bindValue(":rowid", rowid);
    quer.exec();
}

void DbAdapter::updateGroup(qlonglong rowid, QString name, QString color)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people_groups SET name=:name, color=:color WHERE rowid=:rowid");
    query.bindValue(":name", name);
    query.bindValue(":color", color);
    query.bindValue(":rowid", rowid);
    query.exec();
}

void DbAdapter::groupMatrixInsert(qlonglong rowid_people, qlonglong rowid_groups)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_groups_matrix (rowid_people, rowid_groups) "
                  "SELECT :rowid_people, :rowid_groups "
                  "WHERE NOT EXISTS (SELECT 1 FROM people_groups_matrix WHERE rowid_people=:rowid_people AND rowid_groups=:rowid_groups)");
    query.bindValue(":rowid_people", rowid_people);
    query.bindValue(":rowid_groups", rowid_groups);
    query.exec();
}

void DbAdapter::groupMatrixDelete(qlonglong rowid_people, qlonglong rowid_groups)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_groups_matrix WHERE rowid_people=:rowid_people AND rowid_groups=:rowid_groups");
    query.bindValue(":rowid_people", rowid_people);
    query.bindValue(":rowid_groups", rowid_groups);
    query.exec();
}

QList<QMap<QString,QVariant>> DbAdapter::groupMatrixSelect(qlonglong rowid_people)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people_groups.name, people_groups_matrix.rowid_groups, people_groups_matrix.rowid_people"
                  " FROM people_groups_matrix"
                  " JOIN people_groups ON people_groups.rowid=people_groups_matrix.rowid_groups"
                  " WHERE rowid_people=:rowid_people");
    query.bindValue(":rowid_people", rowid_people);
    query.exec();
    
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

QList<QMap<QString,QVariant>> DbAdapter::selectDonationsForReceipt(QString date_from, QString date_to)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people.rowid, people.donorid, people.name, people.address, vcard.value AS geo, donations.amount_sum, donations.code AS currency"
                    " FROM people"
                    " LEFT JOIN people_donations ON people.rowid = people_donations.rowid_people"
                    " LEFT JOIN ("
                        " SELECT people_vcard.rowid_people, people_vcard.key, people_vcard.value"
                        " FROM people_vcard"
                        " WHERE people_vcard.key = 'GEO'"
                        " ) AS vcard"
                        " ON people.rowid = vcard.rowid_people"
                    " LEFT JOIN ("
                        " SELECT people_donations.rowid_people, SUM(people_donations.amount) AS amount_sum, currencies.code"
                        " FROM people_donations"
                        " JOIN currencies"
                        " ON currencies.rowid = people_donations.rowid_currencies"
                        " WHERE people_donations.bank_name NOT NULL"
                        " AND people_donations.date >= DATE(:date_from)"
                        " AND people_donations.date <= DATE(:date_to)"
                        " GROUP BY people_donations.rowid_people"
                        " ) AS donations"
                        " ON people.rowid = donations.rowid_people"
                    " WHERE people_donations.bank_name NOT NULL"
                    " AND people_donations.date >= DATE(:date_from)"
                    " AND people_donations.date <= DATE(:date_to)"
                    " GROUP BY people.name");
    query.bindValue(":date_from", date_from);
    query.bindValue(":date_to", date_to);
    query.exec();
    
    return dbIteratorToMapList(query);
}
QList<QMap<QString,QVariant>> DbAdapter::selectDonationsForPerson(qlonglong people_rowid, QString date_from, QString date_to)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people_donations.date, people_donations.amount, currencies.code"
                  " FROM people_donations"
                  " JOIN currencies ON currencies.rowid = people_donations.rowid_currencies"
                  " WHERE bank_name NOT NULL"
                  " AND people_donations.rowid_people = :people_rowid"
                  " AND people_donations.date >= DATE(:date_from)"
                  " AND people_donations.date <= DATE(:date_to)"
                  " ORDER BY people_donations.date");
    query.bindValue(":people_rowid", people_rowid);
    query.bindValue(":date_from", date_from);
    query.bindValue(":date_to", date_to);
    query.exec();
    
    return dbIteratorToMapList(query);
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
    QSqlQuery query("SELECT rowid, number, subject, cover, content_path, date, date_last_edit FROM mail ORDER BY date DESC", this->db);
    
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
    
    this->db.commit();
}

void DbAdapter::insertSettings(QString key, int value)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:key, :value)");
    query.bindValue(":key", key);
    query.bindValue(":value", value);
    query.exec();
    
    this->db.commit();
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
    QSqlQuery query("SELECT j.rowid, j.name, j.date_from, j.date_to, printf('%.2f', t.costs) AS costs"
                    " FROM journeys j"
                    " LEFT JOIN ("
                        " SELECT rowid_journeys, SUM(cost * currencies.exchange_rate) AS costs"
                    " FROM journey_tickets"
                    " LEFT JOIN currencies ON journey_tickets.rowid_currency=currencies.rowid"
                    " GROUP BY rowid_journeys "
                    ") t ON t.rowid_journeys = j.rowid "
                    "ORDER BY date_from DESC, date_to DESC", this->db);
    
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

void DbAdapter::deleteJourney(qlonglong rowid)
{
    QSqlQuery query_delete_journey(this->db);
    query_delete_journey.prepare("DELETE FROM journeys WHERE rowid=:rowid");
    query_delete_journey.bindValue(":rowid", rowid);
    query_delete_journey.exec();
    
    QSqlQuery query_delete_visit_markers(this->db);
    query_delete_visit_markers.prepare("DELETE FROM people_visits WHERE rowid_journeys=:rowid");
    query_delete_visit_markers.bindValue(":rowid", rowid);
    query_delete_visit_markers.exec();
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

qlonglong DbAdapter::insertVisitWithJourney(qlonglong rowid_journey)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_visits (rowid_journeys) VALUES (:rowid)");
    query.bindValue(":rowid", rowid_journey);
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

qlonglong DbAdapter::insertVisitWithoutJourney()
{
    QSqlQuery query("INSERT INTO people_visits DEFAULT VALUES", this->db);
    
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
                  "LEFT JOIN people ON rowid_people=people.rowid "
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
                  "LEFT JOIN currencies ON rowid_currency=currencies.rowid "
                  "ORDER BY date DESC");
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

QMap<QString,QVariant> DbAdapter::selectCurrency(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT code, exchange_rate, notes FROM currencies WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}

qlonglong DbAdapter::insertCurrency()
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO currencies DEFAULT VALUES");
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::updateCurrency(qlonglong rowid, QString code, QString exchange_rate, QString notes)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE currencies SET code=:code, exchange_rate=:exchange_rate, notes=:notes WHERE rowid=:rowid");
    query.bindValue(":code", code);
    query.bindValue(":exchange_rate", exchange_rate);
    query.bindValue(":notes", notes);
    query.bindValue(":rowid", rowid);
    query.exec();
}

void DbAdapter::deleteCurrency(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM currencies WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
}

qlonglong DbAdapter::currencyROWIDForCode(QString code)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid FROM currencies WHERE code=:code");
    query.bindValue(":code", code);
    query.exec();
    
    return dbIteratorToMap(query)["rowid"].toLongLong();
}

QMap<QString,QVariant> DbAdapter::currencySelectDefault()
{
    QSqlQuery query("SELECT rowid, code FROM currencies WHERE exchange_rate=1 ORDER BY rowid LIMIT 1", this->db);
    
    return dbIteratorToMap(query);
}


QList<QMap<QString,QVariant>> DbAdapter::donationsSelect()
{
    QSqlQuery query("SELECT name, people_donations.rowid, rowid_people, amount, currencies.code, rowid_currencies, people_donations.date "
                  "FROM people_donations "
                  "LEFT JOIN people ON rowid_people=people.rowid "
                  "LEFT JOIN currencies ON rowid_currencies=currencies.rowid "
                  "ORDER BY date DESC", this->db);
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::donationsSelectForPerson(qlonglong rowid_people)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT name, people_donations.rowid, rowid_people, amount, currencies.code, currencies.exchange_rate, rowid_currencies, people_donations.date, memo"
                  " FROM people_donations"
                  " LEFT JOIN people ON rowid_people=people.rowid"
                  " LEFT JOIN currencies ON rowid_currencies=currencies.rowid"
                  " WHERE rowid_people=:rowid_people"
                  " ORDER BY people_donations.date DESC");
    query.bindValue(":rowid_people", rowid_people);
    query.exec();
    
    return dbIteratorToMapList(query);
}
QMap<QString,QVariant> DbAdapter::donationSelect(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT people_donations.rowid, rowid_people, person_name, amount, rowid_currencies, date, memo, tnt_code, mandate_reference, bank_name, people.name FROM people_donations JOIN people ON people_donations.rowid_people = people.rowid WHERE people_donations.rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    return dbIteratorToMap(query);
}
void DbAdapter::donationInsert(QMap<QString, QVariant> data)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_donations (rowid_people, person_name, amount, rowid_currencies, date, memo, tnt_code, bank_name) VALUES (:rowid_people, :person_name, :amount, :rowid_currencies, :date, :memo, :tnt_code, :bank_name)");
    
    query.bindValue(":rowid_people", data["rowid_people"].toString());
    query.bindValue(":person_name", data["person_name"].toString());
    query.bindValue(":amount", data["amount"].toString());
    query.bindValue(":rowid_currencies", data["rowid_currencies"].toString());
    query.bindValue(":date", data["date"].toString());
    query.bindValue(":memo", data["memo"].toString());
    if (data.contains("tnt_code"))
    {
        query.bindValue(":tnt_code", data["tnt_code"].toString());
    }
    else
    {
        query.bindValue(":tnt_code", QVariant(QVariant::String)); // insert NULL
    }
    if (data.contains("bank_name"))
    {
        query.bindValue(":bank_name", data["bank_name"]);
    }
    else
    {
        query.bindValue(":bank_name", QVariant(QVariant::String)); // insert NULL
    }
    query.exec();
}
void DbAdapter::donationUpdate(qlonglong rowid, QMap<QString, QVariant> data)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people_donations SET rowid_people=:rowid_people, person_name=:person_name, amount=:amount, rowid_currencies=:rowid_currencies, date=:date, memo=:memo, tnt_code=:tnt_code, bank_name=:bank_name WHERE rowid=:rowid");
    query.bindValue(":rowid_people", data["rowid_people"].toLongLong());
    query.bindValue(":person_name", data["person_name"].toString());
    query.bindValue(":amount", data["amount"].toString());
    query.bindValue(":rowid_currencies", data["rowid_currencies"].toLongLong());
    query.bindValue(":date", data["date"].toString());
    query.bindValue(":memo", data["memo"].toString());
    query.bindValue(":tnt_code", data["tnt_code"].toString());
    query.bindValue(":bank_name", data["bank_name"].toString());
    query.bindValue(":rowid", rowid);
    query.exec();
}
void DbAdapter::donationDelete(qlonglong rowid_donation)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_donations WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid_donation);
    query.exec();
}
qlonglong DbAdapter::donationNewRow()
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_donations DEFAULT VALUES");
    query.exec();
    
    this->db.commit();
    return query.lastInsertId().toLongLong();
}

bool DbAdapter::donationDoesEntryAlreadyExist(QString person_name, QString amount, QString date, QString memo, QString tnt_code)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT COUNT(*) AS count FROM people_donations WHERE person_name=:person_name AND amount=:amount AND date=:date AND memo=:memo AND tnt_code=:tnt_code");
    query.bindValue(":person_name", person_name);
    query.bindValue(":amount", amount);
    query.bindValue(":date", date);
    query.bindValue(":memo", memo);
    query.bindValue(":tnt_code", tnt_code);
    query.exec();
    
    QMap<QString,QVariant> result = dbIteratorToMap(query);
    if (result["count"].toInt() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QList<QMap<QString,QVariant>> DbAdapter::donationsByMonth()
{
    QSqlQuery query("SELECT SUM(people_donations.amount * exchange_rate) AS amount, strftime('%Y-%m', people_donations.date) as 'year-month' "
                    "FROM people_donations JOIN currencies ON people_donations.rowid_currencies=currencies.rowid "
                    "GROUP BY strftime('%Y-%m', people_donations.date) "
                    "ORDER BY 'year-month' ASC", this->db);
    
    return dbIteratorToMapList(query);
}

qlonglong DbAdapter::relationInsert()
{
    QSqlQuery query("INSERT INTO people_relations DEFAULT VALUES", this->db);

    this->db.commit();
    return query.lastInsertId().toLongLong();
}

void DbAdapter::relationDelete(qlonglong rowid)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_relations WHERE rowid=:rowid");
    query.bindValue(":rowid", rowid);
    query.exec();
    
    QSqlQuery quer(this->db);
    quer.prepare("DELETE FROM people_relations_matrix WHERE rowid_people_relations=:rowid");
    quer.bindValue(":rowid", rowid);
    quer.exec();
}

void DbAdapter::relationMatrixInsert(qlonglong rowid_person_a, qlonglong rowid_person_b, qlonglong rowid_relations)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO people_relations_matrix (rowid_people_a, rowid_people_b, rowid_people_relations) "
                  "SELECT :rowid_person_a, :rowid_person_b, :rowid_relations "
                  "WHERE NOT EXISTS (SELECT 1 FROM people_relations_matrix WHERE rowid_people_a=:rowid_person_a AND rowid_people_b=:rowid_person_b AND rowid_people_relations=:rowid_relations)");
    query.bindValue(":rowid_person_a", rowid_person_a);
    query.bindValue(":rowid_person_b", rowid_person_b);
    query.bindValue(":rowid_relations", rowid_relations);
    query.exec();
}

void DbAdapter::relationMatrixDelete(qlonglong rowid_people_a, qlonglong rowid_people_b, qlonglong rowid_relations)
{
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM people_relations_matrix WHERE rowid_people_a = :rowid_people_a AND rowid_people_b = :rowid_people_b AND rowid_people_relations = :rowid_relations");
    query.bindValue(":rowid_people_a", rowid_people_a);
    query.bindValue(":rowid_people_b", rowid_people_b);
    query.bindValue(":rowid_relations", rowid_relations);
    query.exec();
}

void DbAdapter::relationUpdate(qlonglong rowid, QString name, QString color)
{
    QSqlQuery query(this->db);
    query.prepare("UPDATE people_relations SET name=:name, color=:color WHERE rowid=:rowid");
    query.bindValue(":name", name);
    query.bindValue(":color", color);
    query.bindValue(":rowid", rowid);
    query.exec();
}

QList<QMap<QString,QVariant>> DbAdapter::relationsMatrixSelect(qlonglong rowid_people)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT rowid_people_a, rowid_people_b, p1.name AS name_a, p2.name AS name_b, rowid_people_relations, people_relations.name"
                  " FROM people_relations_matrix"
                  " JOIN people_relations ON people_relations.rowid = people_relations_matrix.rowid_people_relations"
                  " JOIN people AS p1 ON p1.rowid = people_relations_matrix.rowid_people_a"
                  " JOIN people AS p2 ON p2.rowid = people_relations_matrix.rowid_people_b"
                  " WHERE rowid_people_a = :rowid_people OR rowid_people_b = :rowid_people");
    query.bindValue(":rowid_people", rowid_people);
    query.exec();
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::selectRelations()
{
    QSqlQuery query("SELECT people_relations.rowid, people_relations.name, people_relations.color, COUNT(m1.rowid_people_relations) AS count"
                    " FROM people_relations"
                    " LEFT JOIN people_relations_matrix AS m1 ON m1.rowid_people_relations=people_relations.rowid"
                    " GROUP BY people_relations.rowid", this->db);
    
    return dbIteratorToMapList(query);
}

QList<QMap<QString,QVariant>> DbAdapter::SelectTableNames()
{
    QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'", this->db);
    //query.exec();
    
    return dbIteratorToMapList(query);
}
