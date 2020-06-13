#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include <QSettings>
#include <QSize>
#include <QCoreApplication>
#include <QApplication>

#include <QDialog>

#include <QDebug>

#include "firstrun.h"
#include "user_select.h"

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    void showUsernManagementDialog();
    
    QString getDbPath();
    void setDbPath(QString path);
    
private:
    QSettings *settings;
    QString user_name_active;
    
    UserSelect *user_select;
    QDialog *user_select_dialog;
    
    void openConfigFile();
    void initializeWithDefaultValues(QDir *config_dir);
    
    void userNameSelected(QString user_name);
    void newUserAdded(QString user_name);
    void userDeleted(QString user_name);
    
signals:
    
public slots:
};

#endif // CONFIG_H
