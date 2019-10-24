#ifndef MAILMESSAGE_H
#define MAILMESSAGE_H

#include <QObject>
#include <QDebug>

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QByteArray>
#include <QProcess>
#include <QDateTime>

#include <curl/curl.h>
#include <stdio.h>
#include <iostream>

class MailMessage : public QObject
{
    Q_OBJECT
public:
    explicit MailMessage(QObject *parent = nullptr);
    
    void setSMTPPort(QString smtp_port);
    void setSMTPAddress(QString smtp_address);
    void setSMTPUser(QString smtp_user);
    void setSMTPPassword(QString smtp_password);
    
    void addTo(QStringList address_to);
    void setFrom(QString address_from);
    void setReplyTo(QString address_reply);
    void setSubject(QString subject);
    
    void setAlternativeText(QString text);
    void setHTML(QString html);
    void addAttachment(QString attachment_path);
    
    void generateMessage();
    void saveMessage(QString filepath);
    // return POSIX-style: 0: ok; 1: error
    int sendMailWithExternalCURL();
    
    QString message;
    QByteArray message_array;
    unsigned long message_size;
    const char* message_char;
    
    QString smtp_port;
    QString smtp_address;
    QString smtp_user;
    QString smtp_password;
    //QStringList addresses_to;
    QStringList addresses_to;
    QString address_from;
    QString address_reply;
    QString subject;
    QString payload_text;
    QString payload_html;
    QStringList attachments_base64;
    QStringList attachments_filename;
    
    double upload_speed;
    double upload_time;
    
protected:
    static size_t process_mail(void *ptr, size_t size, size_t nmemb, void *userp);
    
signals:
    
public slots:
};

#endif // MAILMESSAGE_H
