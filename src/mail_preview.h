#ifndef MAILPREVIEW_H
#define MAILPREVIEW_H

#include <QObject>
#include <QWidget>

#include <QFile>
#include <QTextStream>
#include <QWebEngineView>

class MailPreview : public QWebEngineView
{
    Q_OBJECT
public:
    explicit MailPreview(QWebEngineView *parent = nullptr);
    
    void updateContent(QString cover_text, QString html_path);
    
private:
    QString html;
    
signals:
    
public slots:
};

#endif // MAILPREVIEW_H
