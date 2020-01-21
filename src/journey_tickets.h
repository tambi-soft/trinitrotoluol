#ifndef JOURNEYTICKETS_H
#define JOURNEYTICKETS_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include <QVBoxLayout>

#include <QPushButton>
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
#include "combo_currencies.h"
#include "lib/grid_widget.h"

class JourneyTickets : public GridWidget
{
    Q_OBJECT
public:
    explicit JourneyTickets(qlonglong rowid_journey, DbAdapter *db, GridWidget *parent = nullptr);
    
private:
    qlonglong rowid_journey;
    DbAdapter *db;
    
    void showData();
    
signals:
    
public slots:
    
private slots:
    void addNewTicket();
    void editTicket(qlonglong rowid);
    void deleteTicket(qlonglong rowid, QString name);
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
    ComboCurrencies *combo_currencies;
    QCheckBox *edit_flag_settled;
    QTextEdit *edit_notes;
    
signals:
    void dataSaved();
    
private slots:
    void saveData();
    void saveDataArg(double /* just for compat */);
    void saveDataInt(int /* just for compat */);
};

#endif // JOURNEYTICKETS_H
