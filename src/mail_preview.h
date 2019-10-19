#ifndef MAILPREVIEW_H
#define MAILPREVIEW_H

#include <QObject>
#include <QWidget>

class MailPreview : public QWidget
{
    Q_OBJECT
public:
    explicit MailPreview(QWidget *parent = nullptr);
    
signals:
    
public slots:
};

#endif // MAILPREVIEW_H
