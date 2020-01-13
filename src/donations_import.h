#ifndef DONATIONSIMPORT_H
#define DONATIONSIMPORT_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class DonationsImport : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsImport(DbAdapter *db, QWidget *parent = nullptr);
    
signals:
    
};

#endif // DONATIONSIMPORT_H
