#ifndef FIRSTRUN_H
#define FIRSTRUN_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "config.h"

class FirstRun : public QDialog
{
    Q_OBJECT
public:
    explicit FirstRun(QString message, QDialog *parent = nullptr);
    
    void setMessage(QString message);
    
private:
    void emitDatabaseSelected(QString db_path);
signals:
    void databasePathSelected(QString db_path);
};



#include <QFileDialog>
#include <QLabel>

class SelectDatabase : public QWidget
{
    Q_OBJECT
public:
    explicit SelectDatabase(QWidget *parent = nullptr);
    
public:
    QLabel *label_message = new QLabel();
    void setMessage(QString message);
    
private:
    
signals:
    void databasePathSelected(QString db_path);
    
public slots:
    void selectExistingFileDialog();
    void selectNewFileDialog();
    
};



class SetupName : public QWidget
{
    Q_OBJECT
public:
    explicit SetupName(QWidget *parent = nullptr);
    
signals:
    
};


#endif // FIRSTRUN_H
