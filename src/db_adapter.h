#ifndef DB_ADAPTER_H
#define DB_ADAPTER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDir>
#include <QDateTime>

#include "config.h"
#include "lib/db_access.h"

class DbAdapter : public DbAccess
{
    Q_OBJECT
    
public:
    explicit DbAdapter(Config *config, DbAccess *parent = nullptr);
    void commit();
    
    QSqlQuery bindPersonParams(QSqlQuery query, QMap<QString, QVariant> data);
    qlonglong insertNewPerson();
    void personInsertTNTID(qlonglong rowid, QString tnt_id);
    //qlonglong insertNewPerson(QMap<QString, QVariant> data);
    void updatePerson(qlonglong rowid, QMap<QString,QVariant> data);
    void deletePerson(qlonglong rowid);
    void deactivatePerson(qlonglong rowid);
    QMap<QString, QVariant> selectPerson(qlonglong rowid);
    qlonglong personRowidForTNTCode(QString code);
    //QList<QMap<QString, QVariant>> selectAllPersons();
    // "todo", "waiting", "donating" and "mail" are tristate: -1: show both, 0: show 0, 1: show = 1 (or with donating: > 0)
    QList<QMap<QString, QVariant>> selectAllPersonsFiltered(int todo, int waiting, int donating, int deactivated, int agreed_mail, QString group, QString name, QString mail);
    QList<QMap<QString, QVariant>> selectAllPersonsForMail(bool agreed_mail);
    QList<QMap<QString, QVariant>> selectVisitsForPerson(qlonglong rowid_person);
    QList<QMap<QString, QVariant>> selectMailsForPerson(qlonglong rowid_person);
    
    QList<QMap<QString, QVariant>> selectGroups();
    qlonglong insertNewGroup();
    void deleteGroup(qlonglong rowid);
    void updateGroup(qlonglong rowid, QString name, QString color);
    
    void groupMatrixInsert(qlonglong rowid_people, qlonglong rowid_groups);
    void groupMatrixDelete(qlonglong rowid_people, qlonglong rowid_groups);
    QList<QMap<QString, QVariant>> groupMatrixSelect(qlonglong rowid_people);

    QMap<QString, QVariant> selectMoneyStats();
    QMap<QString, QVariant> selectPeopleStats();
    
    qlonglong insertNewMail();
    void updateMail(qlonglong rowid, QMap<QString, QVariant> data);
    void deleteMail(qlonglong rowid);
    QMap<QString,QVariant> selectMail(qlonglong rowid);
    QList<QMap<QString,QVariant>> selectAllMails();
    void insertMailSent(qlonglong rowid_mail, qlonglong rowid_people);
    
    void insertSettings(QString key, QString value);
    void insertSettings(QString key, int value);
    QString selectSettings(QString key);
    
    QList<QMap<QString, QVariant>> selectJourneys();
    qlonglong insertJourney(QString name, QString date_from, QString date_to, QString notes);
    void updateJourney(qlonglong rowid, QString name, QString date_from, QString date_to, QString notes);
    QMap<QString,QVariant> selectJourney(qlonglong rowid);
    void deleteJourney(qlonglong rowid);
    
    QList<QMap<QString, QVariant>> selectVisitsForJourney(qlonglong rowid_journey);
    qlonglong insertVisitWithJourney(qlonglong rowid_journey);
    qlonglong insertVisitWithoutJourney();
    void updateVisit(qlonglong rowid, qlonglong rowid_people, QString date, QString notes);
    QMap<QString,QVariant> selectVisit(qlonglong rowid);
    void deleteVisit(qlonglong rowid);
    
    QList<QMap<QString,QVariant>> selectTicketsForJourney(qlonglong rowid_journey);
    QMap<QString,QVariant> selectTicket(qlonglong rowid);
    qlonglong insertTicket(qlonglong rowid_journey);
    void updateTicket(qlonglong rowid, QString name, double cost, qlonglong rowid_currency, bool flag_settled, QString notes);
    void deleteTicket(qlonglong rowid);
    
    QList<QMap<QString,QVariant>> selectExpenses();
    QMap<QString,QVariant> selectExpense(qlonglong rowid);
    qlonglong insertExpense();
    void updateExpense(qlonglong rowid, qlonglong rowid_currency, QMap<QString, QVariant> data);
    void deleteExpense(qlonglong rowid);
    
    QList<QMap<QString,QVariant>> selectCurrencies();
    QMap<QString,QVariant> selectCurrency(qlonglong rowid);
    qlonglong insertCurrency();
    void updateCurrency(qlonglong rowid, QString code, QString exchange_rate, QString notes);
    void deleteCurrency(qlonglong rowid);
    qlonglong currencyROWIDForCode(QString code);
    QMap<QString,QVariant> currencySelectDefault();
    
    QList<QMap<QString, QVariant> > donationsSelect();
    QList<QMap<QString,QVariant>> donationsSelectForPerson(qlonglong rowid_people);
    void donationInsert(QMap<QString,QVariant> data);
    void donationDelete(qlonglong rowid_donation);
    bool donationDoesEntryAlreadyExist(QString person_name, QString amount, QString date, QString memo, QString tnt_code);
    
    qlonglong relationInsert();
    void relationDelete(qlonglong rowid);
    void relationMatrixInsert(qlonglong rowid_person_a, qlonglong rowid_person_b, qlonglong rowid_relations);
    void relationMatrixDelete(qlonglong rowid_people_a, qlonglong rowid_people_b, qlonglong rowid_relations);
    void relationUpdate(qlonglong rowid, QString name, QString color);
    QList<QMap<QString,QVariant>> relationsMatrixSelect(qlonglong rowid_people);
    QList<QMap<QString,QVariant>> selectRelations();
    
    QList<QMap<QString,QVariant>> SelectTableNames();
    
private:
    QSqlDatabase db;
    
    void initializeTables();
    
signals:
    
public slots:
    
};

#endif // DB_ADAPTER_H
