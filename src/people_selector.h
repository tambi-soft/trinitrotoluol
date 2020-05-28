#ifndef PEOPLESELECTOR_H
#define PEOPLESELECTOR_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit>
#include <QComboBox>

#include "db_adapter.h"

class PeopleSelector : public QWidget
{
    Q_OBJECT
public:
    explicit PeopleSelector(DbAdapter *db, QWidget *parent = nullptr);
    
    void setDescription(QString description);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    QComboBox *filter_group;
    QLineEdit *filter_name;
    QLabel *label_description = new QLabel;
    QScrollArea *scroll_area;
    
    void drawData();
    void clearData();
    void filterChanged();
    
signals:
    void personSelected(qlonglong rowid, QString name);
    
private slots:
    void personButtonClicked(qlonglong rowid, QString name);
};

#endif // PEOPLESELECTOR_H
