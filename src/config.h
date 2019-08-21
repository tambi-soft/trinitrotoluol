#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include <QSettings>
#include <QSize>

#include <QDebug>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    QString getDbPath();
    
private:
    QSettings *settings;
    
    void initializeWithDefaultValues(QDir *config_dir);
    
signals:
    
public slots:
};

#endif // CONFIG_H
