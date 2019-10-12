#ifndef JOURNEYEDIT_H
#define JOURNEYEDIT_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QDate>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QMap>

#include "db_adapter.h"
#include "journey_visits.h"
#include "journey_tickets.h"

class JourneyEdit : public QWidget
{
    Q_OBJECT
public:
    //explicit JourneyEdit(DbAdapter *db, QWidget *parent = nullptr);
    explicit JourneyEdit(qlonglong rowid, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    qlonglong rowid;
    DbAdapter *db;
    QVBoxLayout *layout;
    
    QLineEdit *edit_name;
    QDateEdit *edit_date_from;
    QDateEdit *edit_date_to;
    QTextEdit *edit_notes;
    
    void drawGUI();
    void loadData();
    void saveData();
    
signals:
    
public slots:
};

#endif // JOURNEYEDIT_H
