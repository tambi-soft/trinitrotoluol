#ifndef LINEEDITVCARD_H
#define LINEEDITVCARD_H

#include <QObject>
#include <QWidget>

#include <QLineEdit>

#include "db_adapter.h"

class LineEditVCard : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditVCard(qlonglong user_id, DbAdapter *db, qlonglong vcard_id, QString vcard_key, QLineEdit *parent = nullptr);
    
    qlonglong user_id;
    qlonglong vcard_id; // table "people_vcard" column "rowid"
    QString vcard_key;
    
private:
    DbAdapter *db;
    
    void saveText(QString text);
    
signals:
    
};

#endif // LINEEDITVCARD_H
