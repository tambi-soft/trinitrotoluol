#ifndef TOOLCOPYEMAILADDRESSES_H
#define TOOLCOPYEMAILADDRESSES_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>

#include "db_adapter.h"

class ToolCopyEmailAddresses : public QWidget
{
    Q_OBJECT
public:
    explicit ToolCopyEmailAddresses(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    
signals:
    
};

#endif // TOOLCOPYEMAILADDRESSES_H
