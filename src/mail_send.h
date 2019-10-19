#ifndef MAILSEND_H
#define MAILSEND_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

#include <QLabel>
#include <QCheckBox>

#include "db_adapter.h"
#include "mail_preview.h"

class MailSend : public QWidget
{
    Q_OBJECT
public:
    explicit MailSend(DbAdapter *db, qlonglong rowid, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    qlonglong rowid;
    QVBoxLayout *layout = new QVBoxLayout;
    //QHBoxLayout *layout = new QHBoxLayout;
    
    QScrollArea *recipients = new QScrollArea;
    MailPreview *preview = new MailPreview;
    
    void addRecipientsArea();
    void addPreviewArea();
    
signals:
    
public slots:
};

#endif // MAILSEND_H
