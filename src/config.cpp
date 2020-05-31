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
    if (! config_file->exists())
    {
        FirstRun *run = new FirstRun("");
        connect(run, &FirstRun::databasePathSelected, this, &Config::setDbPath);
        run->exec();
    }
    QString db_path = this->settings->value("tnt/db_path").toString();
    QFile *database_file = new QFile(db_path);
    if (! database_file->exists())
    {
        FirstRun *run = new FirstRun("The File \""+db_path+"\" could not be found. Please select a valid database file!");
        connect(run, &FirstRun::databasePathSelected, this, &Config::setDbPath);
        run->exec();
    }
    
    //initializeWithDefaultValues(config_dir);
}

void Config::initializeWithDefaultValues(QDir *config_dir)
{
    QString path = this->settings->value("tnt/db_path").toString();
    if (path == nullptr)
    {
        this->settings->setValue("tnt/db_path", config_dir->absolutePath());
        this->settings->sync();
    }
}

QString Config::getDbPath()
{
    QString result = this->settings->value("tnt/db_path").toString();
    
    return result;
}

void Config::setDbPath(QString path)
{
    this->settings->setValue("tnt/db_path", path);
}
