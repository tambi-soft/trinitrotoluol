#ifndef MAILMESSAGE_H
#define MAILMESSAGE_H

#include <QObject>

class MailMessage : public QObject
{
    Q_OBJECT
public:
    explicit MailMessage(QObject *parent = nullptr);
    
    void setSMTPPort(int smtp_port);
    void setSMTPAddress(QString smtp_address);
    void setSMTPUser(QString smtp_user);
    void setSMTPPassword(QString smtp_password);
    
    void addTo(QString address_to);
    void setFrom(QString address_from);
    void setSubject(QString subject);
    
    void setAlternativeText(QString text);
    void setHTML(QString html);
    void addAttachment(QString attachment_path, QString filename);
    
    void generateMessage();
    // return POSIX-style: 0: ok; 1: error
    int sendMail();
    
    QString message;
    
    int smtp_port;
    QString smtp_address;
    QString smtp_user;
    QString smtp_password;
    //QStringList addresses_to;
    QString address_to;
    QString address_from;
    QString subject;
    QString payload_text;
    QString payload_html;
    QStringList attachments_base64;
    
    
signals:
    
public slots:
};

#endif // MAILMESSAGE_H
