#ifndef PERSONRELATIONSANDGROUPSEDIT_H
#define PERSONRELATIONSANDGROUPSEDIT_H

#include <QObject>
#include <QWidget>

#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QColorDialog>
#include <QColor>

#include "db_adapter.h"
#include "lib/grid_widget.h"

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
    QColor color_new_group;

    void showData();
    void showNewGroupDialog(QString name);
};

#endif // PERSONRELATIONSANDGROUPSEDIT_H
