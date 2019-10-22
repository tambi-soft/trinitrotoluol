#include "mail_message.h"

MailMessage::MailMessage(QObject *parent) : QObject(parent)
{
    
}

void MailMessage::setSMTPPort(QString smtp_port)
{
    this->smtp_port = smtp_port;
}

void MailMessage::setSMTPAddress(QString smtp_address)
{
    if (smtp_address.startsWith("smtp://"))
    {
        this->smtp_address = smtp_address;
    }
    else
    {
        this->smtp_address = "smtp://"+smtp_address;
    }
    
    if (! this->smtp_address.endsWith(":"+this->smtp_port))
    {
        this->smtp_address += ":"+this->smtp_port;
    }
}

void MailMessage::setSMTPUser(QString smtp_user)
{
    this->smtp_user = smtp_user;
}

void MailMessage::setSMTPPassword(QString smtp_password)
{
    this->smtp_password = smtp_password;
}

void MailMessage::addTo(QString address_to)
{
    this->address_to = address_to;
}

void MailMessage::setFrom(QString address_from)
{
    this->address_from = address_from;
}

void MailMessage::setSubject(QString subject)
{
    this->subject = subject;
}

void MailMessage::setAlternativeText(QString text)
{
    this->payload_text = text;
}

void MailMessage::setHTML(QString html)
{
    this->payload_html = html;
}

void MailMessage::addAttachment(QString attachment_path)
{
    //QString base64 = attachment_path.toUtf8().toBase64();
    //this->attachments_base64.append();
    
    QFile file(attachment_path);
    QFileInfo fileInfo(file.fileName());
    QString filename(fileInfo.fileName());
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray blob = file.readAll().toBase64();
    this->attachments_base64.append(QString(blob));
    this->attachments_filename.append(filename);
}

void MailMessage::generateMessage()
{
    this->message = //"To: "+ this->address_to +"\r\n"
            "From: "+ this->address_from +"\r\n" 
            "Subject: "+ this->subject +"\r\n"
            "Content-Type: multipart/mixed; boundary=MixedBoundary\r\n"
            "\r\n"
            "--MixedBoundary\r\n"
            "Content-Type: multipart/alternative; boundary=AlternativeBoundary\r\n"
            "\r\n"
            "--AlternativeBoundary\r\n"
            "Content-Type: text/plain; charset=\"utf-8\"\r\n"
            "\r\n"+ this->payload_text +"\r\n\r\n"
            "--AlternativeBoundary\r\n"
            "Content-Type: text/html; charset=\"utf-8\"\r\n"
            "\r\n"+ this->payload_html +"\r\n"
            "--AlternativeBoundary--\r\n";
            
    for (int i=0; i < this->attachments_base64.length(); ++i)
    {
        this->message.append("\r\n"
            "--MixedBoundary\r\n"
            "Content-Type: application/pdf; name=\""+ this->attachments_filename.at(i) +"\"\r\n"
            "Content-Transfer-Encoding: base64\r\n"
            "Content-Disposition: attachment; filename=\""+ this->attachments_filename.at(i) +"\"\r\n"
            "\r\n");
        
        this->message.append(this->attachments_base64.at(i));
    }
    
    this->message.append("\r\n"
            "--MixedBoundary--\r\n\0");
    
    this->message_array = this->message.toLocal8Bit();
    this->message_char = this->message_array.data();
    this->message_size = strlen(this->message_char);
    qDebug() << this->message_size;
}

void MailMessage::saveMessage(QString filepath)
{
    QString filename = filepath;
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << this->message << endl;
    }
}

size_t MailMessage::process_mail(void *ptr, size_t size, size_t nmemb, void *userprocess)
{
    size_t retcode = 0;
    
    
    MailMessage *self = reinterpret_cast<MailMessage*>(userprocess);
    
    if (strlen(self->message_char) == 0)
        return retcode;
    
    retcode = (size * nmemb >= strlen(self->message_char)) ? strlen(self->message_char) : size * nmemb;
    
    
    self->message_size -= retcode;
    memcpy(ptr, self->message_char, retcode);
    self->message_char += retcode;
    
    
    return retcode;
}

int MailMessage::sendMail()
{
    //qDebug() << this->message;
    
    qDebug() << "smtp_address: " << this->smtp_address.toStdString().c_str();
    qDebug() << "smtp_user: " << this->smtp_user.toStdString().c_str();
    qDebug() << "smtp_password: " << this->smtp_password.toStdString().c_str();
    qDebug() << "address_from: " << this->address_from.toStdString().c_str();
    qDebug() << "address_to: " << this->address_to.toStdString().c_str();
    
    
    
    CURLcode res = CURLE_OK;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    // CURLUSESSL_ALL, CURLUSESSL_NONE, CURLUSESSL_TRY, CURLUSESSL_CONTROL
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_URL, this->smtp_address.toStdString().c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, this->smtp_user.toStdString().c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, this->smtp_password.toStdString().c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, this->address_from.toStdString().c_str());
    
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, this->address_to.toStdString().c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, &process_mail);
    curl_easy_setopt(curl, CURLOPT_READDATA, this);
    
    /* Send the message */ 
    res = curl_easy_perform(curl);
    //CURLM *multi_handle = curl_multi_init();
    //curl_multi_add_handle(multi_handle, curl);
    
    //curl_multi_perform(multi_handle, nullptr);
    /* Check for errors */
    /*
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else
    {
        curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &upload_speed);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &upload_time);
        
        qDebug() << "Speed: "+QString::number(upload_speed) + " Time: "+QString::number(upload_time);
    }
    */
    //curl_slist_free_all(this->address_to.toStdString().c_str());
    //curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl);
    
    return 0;
}
