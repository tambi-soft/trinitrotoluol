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
    QMap<QString,int> layers;
    
    QStackedLayout *stacked_layout;
    
    void onDatabaseSelected(QString db_path);
    
signals:
    void databasePathSelected(QString db_path);
    void signalAbort();
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
    void signalAbort();
    
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



#include "lib/growing_text_edit.h"

class SetupCurrency : public QWidget
{
    Q_OBJECT
public:
    explicit SetupCurrency(QWidget *parent = nullptr);
    
signals:
    
};


#endif // FIRSTRUN_H
