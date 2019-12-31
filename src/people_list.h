#ifndef PEOPLE_LIST_H
#define PEOPLE_LIST_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>

#include <QFrame>
#include <QPushButton>
#include <QIcon>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QScrollBar>

#include "db_adapter.h"

class PeopleList : public QWidget
{
    Q_OBJECT
public:
    explicit PeopleList(DbAdapter *db, QWidget *parent = nullptr);
    void showGroupsFilterCombo();
    void showPeople();
    void refresh();
    // signal to this widget that it should rebuild the view because of the database has changed
    void dataChanged();
    
private:
    QVBoxLayout *layout;
    DbAdapter *db;

    QGridLayout *grid; // inner layout containing the qscrollarea
    QScrollArea *scroll_area = new QScrollArea(this);
    QWidget *scroll_widget = nullptr;
    int scrollbar_pos;
    
    QComboBox *combo_groups;
    QCheckBox *check_todo;
    QCheckBox *check_waiting;
    QCheckBox *check_donating;
    QCheckBox *check_deactivated;
    QCheckBox *check_mail;
    
    QLineEdit *line_name_filter;
    QLineEdit *line_mail_filter;
    
    // see function "dataChanged()". init with "true" for forcing to run at first time
    bool data_changed = true;
    
signals:
    //void addNewPersonSignal();
    void editPersonSignal(qlonglong rowid, QString name);
    void showDonationsForPersonSignal(qlonglong rowid);
    
public slots:
    void onFilterChanged();
    void onNameFilterChanged();
    void onGroupsFilterChanged();
    void onNewPersonButtonClicked();
    void onEditPersonButtonClicked(qlonglong rowid, QString name);
    void onShowDonationsButtonClicked(qlonglong rowid);
    void onDeletePersonButtonClicked(qlonglong rowid, QString name);

    void showEvent(QShowEvent */*event*/);

private slots:
    void scrollBarRangeChanged(int /*min*/, int max);
};

#endif // PEOPLE_LIST_H
