#ifndef COMBOVCARDPROP_H
#define COMBOVCARDPROP_H

#include <QObject>
#include <QWidget>

#include <QComboBox>

#include <QDebug>

class ComboVCardProp : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboVCardProp(QComboBox *parent = nullptr);
    
    QStringList properties_en;
    QStringList properties_standard;
    
private:
    
    
private slots:
    void vcardItemSelected(int index);
    
signals:
    void vcardItemSelectedSignal(QString vcard_item_name);
    
};

#endif // COMBOVCARDPROP_H
