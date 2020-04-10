#ifndef PERSONRELATIONS_H
#define PERSONRELATIONS_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QComboBox>

#include "lib/grid_widget.h"
#include "db_adapter.h"
#include "relations_edit.h"
#include "people_selector.h"

class PersonRelations : public GridWidget
{
    Q_OBJECT
public:
    explicit PersonRelations(DbAdapter *db, qlonglong rowid, GridWidget *parent = nullptr);
    
    void loadRelationsComboData();
    void addPersonToRelation(qlonglong rowid_relations);
    
private:
    DbAdapter *db;
    qlonglong rowid_people;
    
    QMap<QString,int> group_data_map;
    
    QComboBox *combo_relations = new QComboBox;
    QDialog *dialog_select_spouse;
    int relation_selected;
    
    void showData();
    
private slots:
    void onDeleteButtonClicked(qlonglong rowid_a, qlonglong rowid_b, qlonglong rowid_peoples_relations, QString relation, QString name);
    void onEditRelationsButton();
    void onSaveRelation(qlonglong rowid, QString /*name*/);
    
signals:
    
};

#endif // PERSONRELATIONS_H
