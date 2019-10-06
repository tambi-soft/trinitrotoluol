#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QObject>

#include <curl/curl.h>

class MailCurl : public QObject
{
    Q_OBJECT
public:
    explicit MailCurl(QObject *parent = nullptr);
    
private:
    CURL *curl;
    CURLcode res;
    
signals:
    
public slots:
};

#endif // MAILSENDER_H
