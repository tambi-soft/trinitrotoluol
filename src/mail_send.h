#ifndef MAILSEND_H
#define MAILSEND_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

#include <QMessageBox>

#include "db_adapter.h"
#include "mail_preview.h"
#include "mail_message.h"
#include "simplecrypt.h"

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
    
    QMap<QString,QVariant> mail;
    
    QLineEdit *filter_name = new QLineEdit;
    QCheckBox *check_agreed = new QCheckBox("show agreed mail");
    QCheckBox *check_not_agreed = new QCheckBox("show not agreed mail");
    
    QList<QCheckBox*> list_checkboxes;
    QList<QString> list_emails;
    QList<qlonglong> list_people_rowids;
    
    QScrollArea *recipients = new QScrollArea;
    QWidget *recipients_widget;
    QWidget *controls = new QWidget;
    MailPreview *preview = new MailPreview;
    
    void addRecipientsArea();
    void addControlsArea();
    void addPreviewArea();
    
    quint64 KEY = 95839583;
    
signals:
    
public slots:
    void filterForName();
    void filterForAgreed();
    void selectAll();
    void deselectAll();
    void sendMail();
};

#endif // MAILSEND_H
