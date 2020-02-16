#ifndef PERSONRELATIONS_H
#define PERSONRELATIONS_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

#include "lib/grid_widget.h"
#include "db_adapter.h"

class PersonRelations : public GridWidget
{
    Q_OBJECT
public:
    explicit PersonRelations(DbAdapter *db, qlonglong rowid, GridWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid_people;
    
    QMap<QString,int> group_data_map;
    
    void showData();
    
private slots:
    void onDeleteButtonClicked(qlonglong rowid_a, qlonglong rowid_b, qlonglong rowid_peoples_relations, QString relation, QString name);
    void onEditButtonClicked(qlonglong rowid);
    void onNewButtonClicked();
    
signals:
    
};

#endif // PERSONRELATIONS_H
