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
    explicit GroupsAndRelationsEdit(QString new_button_label, QString label_group, GridWidget *parent = nullptr);
    
    void reloadData();
    
    void showData();
    void showNewGroupDialog(QString name);
    
    // to be overloaded
    virtual qlonglong insertTableEntry();
    // to be overloaded
    virtual void updateTableEntry(qlonglong rowid, QString name, QColor color);
    // to be overloaded
    virtual void deleteTableEntry(qlonglong rowid);
    // to be overloaded
    virtual QList<QMap<QString,QVariant>> selectData();
    
private slots:
    void onDeleteButtonClicked(qlonglong group_id, QString group_name, int people_count);
    void onNewGroupButtonClicked();
    void onGroupNameChanged(QString name);
    void onGroupEditNameButton(qlonglong rowid_groups, QString name, QColor color_current);
    void onGroupColorButton(qlonglong rowid_groups, QString name, QColor color_current);

private:
    qlonglong rowid_new_group;
    QColor color_new_group;
    QString label_group;
};

#endif // GROUPSANDRELATIONSEDIT_H
