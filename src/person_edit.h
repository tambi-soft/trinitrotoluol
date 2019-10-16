#ifndef PERSON_EDIT_H
#define PERSON_EDIT_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDialog>

#include "db_adapter.h"
#include "people_selector.h"

class PersonEdit : public QWidget
{
    Q_OBJECT
public:
    explicit PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    
    void drawGUI();
    void loadData();
    void loadGroupsComboData();
    /* pack all data in a qmap for beeing send to the db */
    QMap<QString,QVariant> collectSaveData();
    
private:
    QGridLayout *layout;
    qlonglong rowid = -1;
    DbAdapter *db;
    QList<QMap<QString,QVariant>> group_data;
    QMap<QString,int> group_data_map;
    
    QDialog *dialog_select_spouse;
    
    QCheckBox *check_todo = new QCheckBox;
    QCheckBox *check_waiting = new QCheckBox;
    
    QLineEdit *edit_tnt_id = new QLineEdit;
    QLineEdit *edit_name = new QLineEdit;
    QLineEdit *edit_email = new QLineEdit;
    QLineEdit *edit_address = new QLineEdit;
    QLineEdit *edit_phone = new QLineEdit;
    
    QComboBox *combo_group = new QComboBox;
    
    QCheckBox *check_agreed_mail = new QCheckBox;
    QCheckBox *check_agreed_prayer = new QCheckBox;
    
    QLineEdit *edit_agreement = new QLineEdit;
    QLineEdit *edit_donations_monthly = new QLineEdit;
    QLineEdit *edit_donations_monthly_promised = new QLineEdit;
    
    QLabel *edit_spouse = new QLabel;
    QTextEdit *edit_notes = new QTextEdit;
    
private slots:
    void onAddNewGroupButton();
    void onSelectSpouseButton();
    void onSpouseSelected(qlonglong rowid, QString name);
    void saveDataWithInt(int /* param just for compat */);
    void saveData();
    
signals:
    void closeCurrentTabSignal();
    void dataChanged();
};

#endif // PERSON_EDIT_H
