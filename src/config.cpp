#include "config.h"

Config::Config(QObject *parent) : QObject(parent)
{
    QDir *config_dir = new QDir();
    QFile *config_file = new QFile();
    
    if (QSysInfo::kernelType() == "darwin" || QSysInfo::kernelType() == "linux")
    {
        if (QSysInfo::productType() != "ios" || QSysInfo::productType() != "android")
        {
            config_dir->setPath(QDir::homePath() + "/.trinitrotoluol/");
            if (! config_dir->exists())
            {
                config_dir->mkdir(config_dir->absolutePath());
            }
        }
    }
    else if (QSysInfo::kernelType() == "winnt")
    {
        config_dir = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        if (! config_dir->exists())
        {
            config_dir->mkdir(config_dir->absolutePath());
        }
    }
    
    config_file->setFileName(config_dir->absolutePath() + "/config.ini");
    this->settings = new QSettings(config_file->fileName(), QSettings::IniFormat);
    
    openConfigFile();
    
    if (! this->abortApp)
    {
        if (! config_file->exists())
        {
            FirstRun *run = new FirstRun("");
            connect(run, &FirstRun::databasePathSelected, this, &Config::setDbPath);
            run->exec();
        }
    }
    else
    {
        emit signalAbort();
    }
    //openConfigFile();
}

void Config::openConfigFile()
{
    QStringList keys = this->settings->childGroups();
    keys.removeDuplicates();
    keys.removeAt(keys.indexOf("main"));
    
    if (keys.length() == 1)
    {
        // if there is only one user, just skip the selection of the user
        this->user_name_active = keys.first();
    }
    else
    {
        QString default_user_name = this->settings->value("main/default_user_name").toString();
        if (! default_user_name.isEmpty())
        {
            // we have a default user set in the config
            this->user_name_active = default_user_name;
        }
        else
        {
            // select the user manually
            showUserManagementDialog();
        }
    }
    qDebug() << this->user_name_active;
    
    QString db_path = this->settings->value(this->user_name_active + "/db_path").toString();
    qDebug() << db_path;
    QFile *database_file = new QFile(db_path);
    if (! database_file->exists() && ! abortApp)
    {
        FirstRun *run = new FirstRun("The File \""+db_path+"\" could not be found. Please select a valid database file!");
        connect(run, &FirstRun::databasePathSelected, this, &Config::setDbPath);
        connect(run, &FirstRun::signalAbort, this, [this]{ emit signalAbort(); this->abortApp = true; } );
        switch( run->exec() )
        {
            case QDialog::Rejected:
            {
                this->abortApp = true;
                emit signalAmArsch();
                qDebug() << "cccc";
                emit signalAbort();
                QCoreApplication::quit();
                qApp->exit();
                QApplication::closeAllWindows();
                break;
            }
        }
    }
    if (! database_file->exists())
    {
        this->abortApp = true;
        emit signalAmArsch();
        emit signalAbort();
        QCoreApplication::quit();
        qApp->exit();
        QApplication::closeAllWindows();
    }
    
    //initializeWithDefaultValues(config_dir);
}
void Config::showUserManagementDialog()
{
    QStringList keys = this->settings->childGroups();
    keys.removeDuplicates();
    keys.removeAt(keys.indexOf("main"));
    
    this->user_select = new UserSelect();
    this->user_select->setNames(keys);
    connect(this->user_select, &UserSelect::userSelected, this, &Config::userNameSelected);
    connect(this->user_select, &UserSelect::newUserAdded, this, &Config::newUserAdded);
    connect(this->user_select, &UserSelect::deleteUser, this, &Config::userDeleted);
    
    this->user_select_dialog = new QDialog;
    this->user_select_dialog->setLayout(new QHBoxLayout);
    this->user_select_dialog->layout()->addWidget(this->user_select);
    switch (this->user_select_dialog->exec())
    {
        case QDialog::Rejected:
        {
            this->abortApp = true;
            qDebug() << "aaaaaa";
            //QCoreApplication::quit();
            emit signalAbort();
            qDebug() << "bbbbb";
            
            break;
        }
    }
}
void Config::userNameSelected(QString user_name)
{
    this->user_name_active = user_name;
    
    this->user_select_dialog->close();
}
void Config::newUserAdded(QString user_name)
{
    // initialize an empty init-entry for this user
    this->settings->setValue(user_name + "/db_path", "");
    
    QStringList keys = this->settings->childGroups();
    keys.removeDuplicates();
    keys.removeAt(keys.indexOf("main"));
    
    this->user_select->setNames(keys);
}
void Config::userDeleted(QString user_name)
{
    this->settings->remove(user_name);
    
    QStringList keys = this->settings->childGroups();
    keys.removeDuplicates();
    keys.removeAt(keys.indexOf("main"));
    
    this->user_select->setNames(keys);
}

void Config::initializeWithDefaultValues(QDir *config_dir)
{
    QString path = this->settings->value(this->user_name_active + "/db_path").toString();
    if (path == nullptr)
    {
        this->settings->setValue(this->user_name_active + "/db_path", config_dir->absolutePath());
        this->settings->sync();
    }
}

QString Config::getDbPath()
{
    QString result = this->settings->value(this->user_name_active + "/db_path").toString();
    
    return result;
}

void Config::setDbPath(QString path)
{
    this->settings->setValue(this->user_name_active + "/db_path", path);
}
