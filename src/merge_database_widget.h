#ifndef MERGEDATABASEWIDGET_H
#define MERGEDATABASEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>

#include "db_adapter.h"

class MergeDatabaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MergeDatabaseWidget(DbAdapter *db, QWidget *parent = nullptr);
    
    DbAdapter *db;
    
private:
    void showFileOpenDialog();
    
    
signals:
    void signalCloseTab();
};

#endif // MERGEDATABASEWIDGET_H
