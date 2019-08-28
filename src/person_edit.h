#ifndef PERSON_EDIT_H
#define PERSON_EDIT_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class PersonEdit : public QWidget
{
    Q_OBJECT
public:
    explicit PersonEdit(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
};

#endif // PERSON_EDIT_H
