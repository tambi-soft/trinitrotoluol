#ifndef GROUPSANDRELATIONSEDIT_H
#define GROUPSANDRELATIONSEDIT_H

#include <QObject>
#include <QWidget>

#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QColorDialog>
#include <QColor>

#include "grid_widget.h"

class GroupsAndRelationsEdit : public GridWidget
{
    Q_OBJECT
public:
    explicit GroupsAndRelationsEdit(QString new_button_label, GridWidget *parent = nullptr);
    
    void reloadData();
    
private slots:
    void onDeleteButtonClicked(qlonglong group_id, QString group_name, int people_count);
    void onNewGroupButtonClicked();
    void onGroupNameChanged(QString name);
    void onGroupEditNameButton(qlonglong rowid_groups, QString name, QColor color_current);
    void onGroupColorButton(qlonglong rowid_groups, QString name, QColor color_current);
    

private:
    qlonglong rowid_new_group;
    QColor color_new_group;

    void showData();
    void showNewGroupDialog(QString name);
    
    // to be overloaded
    qlonglong insertTableEntry();
    // to be overloaded
    void updateTableEntry(qlonglong rowid, QString name, QString color);
    // to be overloaded
    void deleteTableEntry(qlonglong rowid);
    // to be overloaded
    QList<QMap<QString,QVariant>> selectData();
};

#endif // GROUPSANDRELATIONSEDIT_H
