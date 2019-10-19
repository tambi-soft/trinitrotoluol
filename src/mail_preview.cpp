#include "mail_preview.h"

MailPreview::MailPreview(QWebEngineView *parent)
    : QWebEngineView(parent)
{
    
}

void MailPreview::updateContent(QString cover_text, QString html_path)
{
    QFile file(html_path);
    if(!file.open(QIODevice::ReadOnly)) {
        //QMessageBox::information(nullptr, "error", file.errorString());
    }
    
    QTextStream in(&file);
    
    while(!in.atEnd()) {
        QString line = in.readLine();
        
        if (line.trimmed() == "<!-- INCLUDE COVER -->")
        {
            this->html.append(cover_text);
        }
        else
        {
            this->html.append(line);
        }
    }
    
    file.close();
    
    setHtml(this->html);
    
    //qDebug() << html;
}
