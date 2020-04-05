#ifndef RELATIONSEDIT_H
#define RELATIONSEDIT_H

#include <QObject>
#include <QWidget>
#include <QColor>

#include "db_adapter.h"
#include "lib/groups_and_relations_edit.h"

class RelationsEdit : public GroupsAndRelationsEdit
{
    Q_OBJECT
public:
    explicit RelationsEdit(DbAdapter *db, GroupsAndRelationsEdit *parent = nullptr);
    
signals:
    
private:
    DbAdapter *db;
    
    // overloaded
    qlonglong insertTableEntry();
    // overloaded
    void updateTableEntry(qlonglong rowid, QString name, QColor color);
    // overloaded
    void deleteTableEntry(qlonglong rowid);
    // overloaded
    QList<QMap<QString,QVariant>> selectData();
};

#endif // RELATIONSEDIT_H
