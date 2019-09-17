#ifndef MAILLIST_H
#define MAILLIST_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QShowEvent>

#include <QList>
#include <QMap>

#include "db_adapter.h"
#include "mail_new.h"

class MailList : public QWidget
{
    Q_OBJECT
public:
    explicit MailList(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    QTableWidget *table;
    void initView();
    QList<QMap<QString,QVariant>> data;
    
signals:
    void closeCurrentTabSignal();
    void signalNewMail();
    
public slots:
    void onNewMailButton();
    void onDeleteMail(qlonglong rowid, QString subject);
    void onEditMail(qlonglong rowid);
    void showEvent(QShowEvent *event);
};

#endif // MAILLIST_H
