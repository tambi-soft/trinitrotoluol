#include "relations_edit.h"

RelationsEdit::RelationsEdit(DbAdapter *db, GroupsAndRelationsEdit *parent) : GroupsAndRelationsEdit("add new relation", "<b>Relation Name</b>", parent)
{
    this->db = db;
    
    showData();
}

// overloaded
qlonglong RelationsEdit::insertTableEntry()
{
    return this->db->relationInsert();
}

// overloaded
void RelationsEdit::updateTableEntry(qlonglong rowid, QString name, QColor color)
{
    //this->db->updateGroup(rowid_groups, name, color.name());
    this->db->relationUpdate(rowid, name, color.name());
}

// overloaded
void RelationsEdit::deleteTableEntry(qlonglong rowid)
{
    //this->db->deleteGroup(group_id);
    this->db->relationDelete(rowid);
    //this->db->relationDelete(rowid_people_a, rowid_people_b_, rowid_relations);
}

// overloaded
QList<QMap<QString,QVariant>> RelationsEdit::selectData()
{
    QList<QMap<QString,QVariant>> result = this->db->selectRelations();
    return result;
}
