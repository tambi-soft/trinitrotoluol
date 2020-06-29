#ifndef VCARD_H
#define VCARD_H

#include <QObject>
#include <QWidget>

class VCardExport : public QWidget
{
    Q_OBJECT
public:
    explicit VCardExport(QWidget *parent = nullptr);
    
signals:
    
};



class VCardImport : public QWidget
{
    Q_OBJECT
public:
    explicit VCardImport(QWidget *parent = nullptr);
    
signals:
    
};



class VCard : public QObject
{
    Q_OBJECT
public:
    explicit VCard(QObject *parent = nullptr);
    
signals:
    
};

#endif // VCARD_H
