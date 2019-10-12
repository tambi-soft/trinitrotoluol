#ifndef JOURNEYVISITS_H
#define JOURNEYVISITS_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include <QVBoxLayout>

#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDateEdit>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>

#include <QMap>
#include <QList>
#include <QDate>

#include "db_adapter.h"

class JourneyVisits : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyVisits(qlonglong rowid_journey, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_journey;
    DbAdapter *db;
    QTableWidget *table;
    QVBoxLayout *layout;
    
    void loadData();
    
signals:
    
public slots:
    
private slots:
    void addNewVisit();
    void editVisit(qlonglong rowid_visits);
    void deleteVisit(qlonglong rowid_visits, QString name);
};

class JourneyVisitsNew : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyVisitsNew(qlonglong rowid_visits, qlonglong rowid_journey, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_visits;
    qlonglong rowid_journey;
    DbAdapter *db;
    QVBoxLayout *layout;
    
    QLineEdit *edit_name;
    QDateEdit *edit_date;
    QTextEdit *edit_notes;
    
private slots:
    void saveData();
};
#endif // JOURNEYVISITS_H
