#ifndef JOURNEYVISITS_H
#define JOURNEYVISITS_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include <QVBoxLayout>

#include <QPushButton>
#include <QAbstractItemView>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>

#include <QMap>
#include <QList>
#include <QDate>

#include "db_adapter.h"
#include "lib/grid_widget.h"
#include "journey_visits_edit.h"

class JourneyVisits : public GridWidget
{
    Q_OBJECT
public:
    explicit JourneyVisits(qlonglong rowid_journey, DbAdapter *db, QString date_hint, GridWidget *parent = nullptr);
    void setDateHint(QString date);
    
    void showData();
    
private:
    qlonglong rowid_journey;
    DbAdapter *db;
    
    QString date_hint;
    
signals:
    
public slots:
    
private slots:
    void addNewVisit();
    void editVisit(qlonglong rowid_visits);
    void deleteVisit(qlonglong rowid_visits, QString name);
};

#endif // JOURNEYVISITS_H
