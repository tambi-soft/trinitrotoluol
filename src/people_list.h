#ifndef PEOPLE_LIST_H
#define PEOPLE_LIST_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QPushButton>
#include <QIcon>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

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
    QLineEdit *line_name_filter;
    QLineEdit *line_mail_filter;
    
signals:
    void addNewPersonSignal();
    void editPersonSignal(qlonglong rowid, QString name);
    void showDonationsForPersonSignal(qlonglong rowid);
    
public slots:
    void onNameFilterChanged();
    void onGroupsFilterChanged();
    void onNewPersonButtonClicked();
    void onEditPersonButtonClicked(qlonglong rowid, QString name);
    void onShowDonationsButtonClicked(qlonglong rowid);
    void onDeletePersonButtonClicked(qlonglong rowid, QString name);
    
    void showEvent(QShowEvent */*event*/);
};

#endif // PEOPLE_LIST_H
