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
#include "groups_edit.h"
#include "help_message.h"

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
    void onEditGroupsButton();
    
private:
    DbAdapter *db;
    qlonglong rowid_people;
    
    QComboBox *combo_group = new QComboBox;
    QMap<QString,int> group_data_map;
    
signals:
    
};

#endif // PERSONGROUPS_H
