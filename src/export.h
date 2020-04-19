#ifndef EXPORT_H
#define EXPORT_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>

#include "db_adapter.h"

class Export : public QWidget
{
    Q_OBJECT
public:
    explicit Export(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    
    void addSelectSource();
    
signals:
    
};

#endif // EXPORT_H
