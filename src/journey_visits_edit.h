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
    explicit JourneyVisitsEdit(qlonglong rowid_visits, qlonglong rowid_person, DbAdapter *db, QString date_hint, QWidget *parent = nullptr);
    
private:
    qlonglong rowid_visits;
    DbAdapter *db;
    
    QVBoxLayout *layout = new QVBoxLayout;
    
    QPushButton *button_name = new QPushButton;
    QDateEdit *edit_date = new QDateEdit;
    QTextEdit *edit_notes = new QTextEdit;
    QDialog *dialog_select_person = new QDialog;
    qlonglong rowid_person = -1;
    
    void initializeView(QString date_hint);
    
private slots:
    void selectPerson();
    void onPersonSelected(qlonglong rowid, QString name);
    void saveData();
    
signals:
    void signalReload();
};

#endif // JOURNEYVISITSEDIT_H
