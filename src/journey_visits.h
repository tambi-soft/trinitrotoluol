#ifndef JOURNEYVISITS_H
#define JOURNEYVISITS_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include <QVBoxLayout>

#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QDateEdit>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>

#include <QMap>
#include <QList>
#include <QDate>

#include "db_adapter.h"
#include "people_selector.h"

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
    void reloadData();
    
signals:
    
public slots:
    
private slots:
    void addNewVisit();
    void editVisit(qlonglong rowid_visits);
    void deleteVisit(qlonglong rowid_visits, QString name);
};

class JourneyVisitsEdit : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyVisitsEdit(qlonglong rowid_visits, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_visits;
    qlonglong rowid_journey;
    DbAdapter *db;
    QVBoxLayout *layout;
    
    QPushButton *button_name;
    QDateEdit *edit_date;
    QTextEdit *edit_notes;
    QDialog *dialog_select_person;
    qlonglong rowid_person;
    
private slots:
    void selectPerson();
    void onPersonSelected(qlonglong rowid, QString name);
    void saveData();
    
signals:
    void signalReload();
};
#endif // JOURNEYVISITS_H
