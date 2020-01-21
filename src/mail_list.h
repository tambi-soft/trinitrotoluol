#ifndef MAILLIST_H
#define MAILLIST_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QShowEvent>
#include <QLabel>

#include <QList>
#include <QMap>

#include "db_adapter.h"
#include "mail_edit.h"
#include "lib/grid_widget.h"

class MailList : public GridWidget
{
    Q_OBJECT
public:
    explicit MailList(DbAdapter *db, GridWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QList<QMap<QString,QVariant>> data;
    
    void showData();
    
signals:
    void signalEditMail(qlonglong rowid);
    void signalSendMail(qlonglong rowid, QString number);
    
public slots:
    void onNewMail();
    void onDeleteMail(qlonglong rowid, QString subject);
    void onEditMail(qlonglong rowid);
    void onSendMail(qlonglong rowid, QString number);
    void showEvent(QShowEvent *event);
};

#endif // MAILLIST_H
