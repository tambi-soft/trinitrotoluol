#ifndef PERSONVISITS_H
#define PERSONVISITS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>

#include "lib/grid_widget.h"
#include "db_adapter.h"
#include "journey_visits_edit.h"

class PersonVisits : public GridWidget
{
    Q_OBJECT
public:
    explicit PersonVisits(DbAdapter *db, qlonglong rowid, GridWidget *parent = nullptr);

private:
    DbAdapter *db;
    qlonglong rowid_person;
    
    void showData();
    
private slots:
    void onDeleteButtonClicked(qlonglong rowid, QString name);
    void onEditButtonClicked(qlonglong rowid_visits);
    void onNewButtonClicked();
    
};

#endif // PERSONVISITS_H
