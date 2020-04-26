#ifndef DONATIONSIMPORTPROGRESS_H
#define DONATIONSIMPORTPROGRESS_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QVBoxLayout>

#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QDialog>
#include <QGroupBox>
#include <QMessageBox>
#include <QCheckBox>

#include <QDate>

#include "db_adapter.h"
#include "people_selector.h"

class DonationsImportProgress : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsImportProgress(DbAdapter *db, QList<QMap<QString,QString>> data, QMap<QString,QString> data_map, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QString person_name;
    QString memo;
    QString tnt_code;
    QString tnt_donor_code;
    
    qlonglong rowid_people;
    qlonglong rowid_currency;
    
    QVBoxLayout *layout = new QVBoxLayout;
    
    QTextEdit *edit_log = new QTextEdit;
    
    QCheckBox *check_save_selection;
    
    void selectPerson(QString name, QString memo);
    QDialog *dialog_select_person;
    
signals:
    
private slots:
    void onPersonSelected(qlonglong rowid_person, QString name_person);
    
};

#endif // DONATIONSIMPORTPROGRESS_H
