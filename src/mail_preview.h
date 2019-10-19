#ifndef MAILPREVIEW_H
#define MAILPREVIEW_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

class MailPreview : public QWidget
{
    Q_OBJECT
public:
    explicit MailPreview(QWidget *parent = nullptr);
    
private:
    QVBoxLayout *layout = new QVBoxLayout;
    
signals:
    
public slots:
};

#endif // MAILPREVIEW_H
