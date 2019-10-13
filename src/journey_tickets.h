#ifndef JOURNEYTICKETS_H
#define JOURNEYTICKETS_H

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
#include <QCheckBox>
#include <QDoubleSpinBox>

#include <QMap>
#include <QList>
#include <QDate>

#include "db_adapter.h"

class JourneyTickets : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyTickets(qlonglong rowid_journey, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_journey;
    DbAdapter *db;
    QTableWidget *table;
    QVBoxLayout *layout;
    
    void loadData();
    
signals:
    
public slots:
    
private slots:
    void addNewTicket();
    void editTicket(qlonglong rowid);
    void deleteTicket(qlonglong rowid, QString name);
    void reloadData();
};

class JourneyTicketsEdit : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyTicketsEdit(qlonglong rowid, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid;
    DbAdapter *db;
    QVBoxLayout *layout;
    
    QLineEdit *edit_name;
    QDoubleSpinBox *edit_cost;
    QCheckBox *edit_flag_settled;
    QTextEdit *edit_notes;
    
signals:
    void dataSaved();
    
private slots:
    void saveData();
    void saveDataArg(double /* just for compat */);
};

#endif // JOURNEYTICKETS_H
