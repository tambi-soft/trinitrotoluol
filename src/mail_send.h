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
#include <QPushButton>

#include <QMessageBox>

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
    QGridLayout *grid = new QGridLayout;
    //QVBoxLayout *layout = new QVBoxLayout;
    //QHBoxLayout *layout = new QHBoxLayout;
    
    bool flag_mail_agreed = true;
    QList<QCheckBox*> list_checkboxes;
    QList<QString> list_emails;
    
    QScrollArea *recipients = new QScrollArea;
    QWidget *recipients_widget;
    QWidget *controls = new QWidget;
    MailPreview *preview = new MailPreview;
    
    void addRecipientsArea();
    void addControlsArea();
    void addPreviewArea();
    
signals:
    
public slots:
    void showAgreed();
    void showNotAgreed();
    void selectAll();
    void deselectAll();
    void sendMail();
};

#endif // MAILSEND_H
