#ifndef GROUPSEDIT_H
#define GROUPSEDIT_H

#include <QObject>
#include <QWidget>

#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QColorDialog>
#include <QColor>

#include "db_adapter.h"
#include "lib/groups_and_relations_edit.h"

class GroupsEdit : public GroupsAndRelationsEdit
{
    Q_OBJECT
public:
    explicit GroupsEdit(DbAdapter *db, GroupsAndRelationsEdit *parent = nullptr);
    
private slots:

private:
    DbAdapter *db;
    
    // overloaded
    qlonglong insertTableEntry();
    // overloaded
    void updateTableEntry(qlonglong rowid_groups, QString name, QColor color);
    // overloaded
    void deleteTableEntry(qlonglong group_id);
    // overloaded
    QList<QMap<QString,QVariant>> selectData();
    
};

#endif // GROUPSEDIT_H
