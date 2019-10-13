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

class DbAdapter : public QObject
{
    Q_OBJECT
    
public:
    explicit DbAdapter(Config *config, QObject *parent = nullptr);
    QSqlQuery bindPersonParams(QSqlQuery query, QMap<QString, QVariant> data);
    qlonglong insertNewPerson(QMap<QString, QVariant> data);
    void updatePerson(qlonglong rowid, QMap<QString,QVariant> data);
    void deletePerson(qlonglong rowid);
    QMap<QString, QVariant> selectPerson(qlonglong rowid);
    //QList<QMap<QString, QVariant>> selectAllPersons();
    // "todo", "waiting", "donating" and "mail" are tristate: -1: show both, 0: show 0, 1: show = 1 (or with donating: > 0)
    QList<QMap<QString, QVariant>> selectAllPersonsFiltered(int todo, int waiting, int donating, int deactivated, int agreed_mail, QString group, QString name, QString mail);
    QList<QMap<QString, QVariant>> selectGroups();
    QMap<QString, QVariant> selectMoneyStats();
    QMap<QString, QVariant> selectPeopleStats();
    
    QSqlQuery bindMailParams(QSqlQuery query, QMap<QString,QVariant> data);
    void insertNewMail(QMap<QString,QVariant> data);
    void deleteMail(qlonglong rowid);
    QMap<QString,QVariant> selectMail(qlonglong rowid);
    QList<QMap<QString,QVariant>> selectAllMails();
    
    void insertSettings(QString key, QString value);
    void insertSettings(QString key, int value);
    QString selectSettings(QString key);
    
    QList<QMap<QString, QVariant>> selectJourneys();
    qlonglong insertJourney(QString name, QString date_from, QString date_to, QString notes);
    void updateJourney(qlonglong rowid, QString name, QString date_from, QString date_to, QString notes);
    QMap<QString,QVariant> selectJourney(qlonglong rowid);
    
    QList<QMap<QString, QVariant>> selectVisitsForJourney(qlonglong rowid_journey);
    qlonglong insertVisit(qlonglong rowid_journey);
    void updateVisit(qlonglong rowid, QString name, QString date, QString notes);
    QMap<QString,QVariant> selectVisit(qlonglong rowid);
    void deleteVisit(qlonglong rowid);
    
    QList<QMap<QString,QVariant>> selectTicketsForJourney(qlonglong rowid_journey);
    QMap<QString,QVariant> selectTicket(qlonglong rowid);
    qlonglong insertTicket(qlonglong rowid_journey);
    void updateTicket(qlonglong rowid, QString name, double cost, bool flag_settled, QString notes);
    void deleteTicket(qlonglong rowid);
    
private:
    QSqlDatabase db;
    
    QList<QMap<QString,QVariant>> dbIteratorToMapList(QSqlQuery query);
    QMap<QString, QVariant> dbIteratorToMap(QSqlQuery query);
    
    void initializeTables();
    
signals:
    
public slots:
    
};

#endif // DB_ADAPTER_H
