#ifndef JOURNEYVISITSEDIT_H
#define JOURNEYVISITSEDIT_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QTextEdit>
#include <QDateEdit>

#include "db_adapter.h"
#include "people_selector.h"

class JourneyVisitsEdit : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyVisitsEdit(qlonglong rowid_visits, DbAdapter *db, QString date_hint, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_visits;
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

#endif // JOURNEYVISITSEDIT_H
