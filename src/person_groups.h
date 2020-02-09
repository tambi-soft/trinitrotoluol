#ifndef PERSONGROUPS_H
#define PERSONGROUPS_H

#include <QObject>
#include <QWidget>

#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMessageBox>

#include <QList>
#include <QMap>

#include "lib/grid_widget.h"
#include "db_adapter.h"

class PersonGroups : public GridWidget
{
    Q_OBJECT
public:
    explicit PersonGroups(DbAdapter *db, qlonglong rowid_people, GridWidget *parent = nullptr);
    
    void showData();
    void deletePersonFromGroup(qlonglong rowid_groups);
    void addPersonToGroup(qlonglong rowid_groups);
    void loadGroupsComboData();
    
private slots:
    void onAddNewGroupButton();
    
private:
    DbAdapter *db;
    qlonglong rowid_people;
    
    QComboBox *combo_group = new QComboBox;
    QMap<QString,int> group_data_map;
    
signals:
    
};



#include <QColorDialog>
#include <QColor>

class GroupsEdit : public GridWidget
{
    Q_OBJECT
public:
    explicit GroupsEdit(DbAdapter *db, GridWidget *parent = nullptr);
    void reloadData();
    
private slots:
    void onDeleteButtonClicked(qlonglong group_id, QString group_name, int people_count);
    void onNewGroupButtonClicked();
    void onGroupNameChanged(QString name);
    void onGroupEditNameButton(qlonglong rowid_groups, QString name, QColor color_current);
    void onGroupColorButton(qlonglong rowid_groups, QString name, QColor color_current);
    

private:
    DbAdapter *db;
    qlonglong rowid_new_group;

    void showData();
};

#endif // PERSONGROUPS_H
