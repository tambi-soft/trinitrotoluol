#ifndef VCARD_H
#define VCARD_H

#include <QObject>

class vcard : public QObject
{
    Q_OBJECT
public:
    explicit vcard(QObject *parent = nullptr);
    
signals:
    
};

#endif // VCARD_H
