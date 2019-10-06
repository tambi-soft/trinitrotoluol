#include "mail_curl.h"

MailCurl::MailCurl(QObject *parent) : QObject(parent)
{
    
    this->curl = curl_easy_init();
}
