#ifndef PERSON_EDIT_H
#define PERSON_EDIT_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>

#include "db_adapter.h"

class PersonEdit : public QWidget
{
    Q_OBJECT
public:
    explicit PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    
    void drawGUI();
    void populateGUI();
    void clear();
private:
    QGridLayout *layout;
    qlonglong rowid;
    DbAdapter *db;
};

#endif // PERSON_EDIT_H
