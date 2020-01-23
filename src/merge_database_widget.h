#ifndef MERGEDATABASEWIDGET_H
#define MERGEDATABASEWIDGET_H

#include <QObject>
#include <QWidget>

#include "db_adapter.h"

class MergeDatabaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MergeDatabaseWidget(DbAdapter *db, QWidget *parent = nullptr);
    
    DbAdapter *db;
    
private:
    
    
signals:
    
};

#endif // MERGEDATABASEWIDGET_H
