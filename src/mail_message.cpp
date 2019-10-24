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
}

void MailMessage::setSMTPUser(QString smtp_user)
{
    this->smtp_user = smtp_user;
}

void MailMessage::setSMTPPassword(QString smtp_password)
{
    this->smtp_password = smtp_password;
}

void MailMessage::addTo(QStringList address_to)
{
    this->addresses_to.append(address_to);
}

void MailMessage::setFrom(QString address_from)
{
    this->address_from = address_from;
}

void MailMessage::setReplyTo(QString address_reply)
{
    this->address_reply = address_reply;
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
    // RFC 2045: lines must not be longer than ~1000 chars
    QString base = html.toUtf8().toBase64();
    int step = 100;
    for (int i = step; i <= base.size(); i+=step+1)
            base.insert(i, "\r\n");
    
    this->payload_html = base;
}

void MailMessage::addAttachment(QString attachment_path)
{
    QFile file(attachment_path);
    
    QFileInfo fileInfo(file.fileName());
    QString filename(fileInfo.fileName());
    
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray blob = file.readAll().toBase64();
    QString blob_str(blob);
    
    int step = 100;
    for (int i = step; i <= blob_str.size(); i+=step+1)
            blob_str.insert(i, "\r\n");
    
    this->attachments_base64.append(blob_str);
    this->attachments_filename.append(filename);
}

void MailMessage::generateMessage()
{
    this->message = //"To: "+ this->address_to +"\r\n"
            "From: "+ this->address_from +"\r\n";
    
    if (this->address_reply.contains("@"))
    {
        this->message.append("Reply-To: "+ this->address_reply +"\r\n");
    }
    this->message.append(
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
                "Content-Transfer-Encoding: base64\r\n"
            "\r\n"+ this->payload_html +"\r\n\r\n"
            "--AlternativeBoundary--\r\n");
            
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
    
    this->message.append("\r\n\r\n"
            "--MixedBoundary--\r\n");
    
    this->message_array = this->message.toLocal8Bit();
    this->message_char = this->message_array.data();
    this->message_size = strlen(this->message_char);
    //qDebug() << this->message_size;
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

int MailMessage::sendMailWithExternalCURL()
{
    QDir dir = QDir::root();
    dir.mkdir("/tmp/trinitrotoluol");
    
    QDateTime time = QDateTime::currentDateTime();
    
    QString path = "/tmp/trinitrotoluol/outmail_"+time.toString("yyyy.MM.dd_HH:mm:ss:zzz")+".eml";
    QFile file;
    file.remove(path);
    saveMessage(path);
    
    QProcess *process = new QProcess(this);
    QString program = "curl";
    QString args = "-v \""+this->smtp_address+":"+this->smtp_port+"\" "
                   "-u \""+ this->smtp_user+":"+this->smtp_password +"\" "
                   "--mail-from \""+this->address_from+"\" ";
    foreach (QString address, this->addresses_to) {
        args.append("--mail-rcpt \""+address+"\" ");
    }
    args.append(   "-T "+path+" "
                   "--ssl-reqd "
                   "--show-error");
    
    process->execute(program + " " + args);
    int code = 0;
    code = process->exitCode();
    
    QString output_err(process->readAllStandardError());
    qDebug() << output_err;
    
    QString output_std(process->readAllStandardOutput());
    qDebug() << output_std;
    
    return code;
}
