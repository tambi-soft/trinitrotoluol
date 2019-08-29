#ifndef PEOPLE_LIST_H
#define PEOPLE_LIST_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QComboBox>
#include <QLineEdit>

#include "db_adapter.h"

class PeopleList : public QWidget
{
    Q_OBJECT
public:
    explicit PeopleList(DbAdapter *db, QWidget *parent = nullptr);
    void showGroupsFilterCombo();
    void showPeople();
    void clear();
    
private:
    QVBoxLayout *layout;
    QTableWidget *table_widget = new QTableWidget();
    DbAdapter *db;
    QComboBox *combo_groups;
    QLineEdit *line_edit_filter;
    
signals:
    void editPersonSignal(qlonglong rowid, QString name);
    void showDonationsForPersonSignal(qlonglong rowid);
    
public slots:
    void onGroupsFilterChanged();
    void onNewPersonButtonClicked();
    void onEditPersonButtonClicked(qlonglong rowid, QString name);
    void onShowDonationsButtonClicked(qlonglong rowid);
};

#endif // PEOPLE_LIST_H
