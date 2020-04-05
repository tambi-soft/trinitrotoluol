#include "groups_edit.h"


GroupsEdit::GroupsEdit(DbAdapter *db, GroupsAndRelationsEdit *parent) : GroupsAndRelationsEdit("add new group", "<b>Group Name</b>", parent)
{
    this->db = db;
    
    showData();
}

// overloaded
qlonglong GroupsEdit::insertTableEntry()
{
    return this->db->insertNewGroup();
}

// overloaded
void GroupsEdit::updateTableEntry(qlonglong rowid_groups, QString name, QColor color)
{
    this->db->updateGroup(rowid_groups, name, color.name());
}

// overloaded
void GroupsEdit::deleteTableEntry(qlonglong group_id)
{
    this->db->deleteGroup(group_id);
}

// overloaded
QList<QMap<QString,QVariant>> GroupsEdit::selectData()
{
    QList<QMap<QString,QVariant>> result = this->db->selectGroups();
    return result;
}
