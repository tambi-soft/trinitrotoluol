#ifndef REST_ADAPTER_H
#define REST_ADAPTER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSysInfo>

class RESTAdapter : public QObject
{
    Q_OBJECT
public:
    explicit RESTAdapter(QObject *parent = nullptr);
    void post(QString endpoint, QStringList params, QMap<QString, QVariant> data);
    void put(QString endpoint, QStringList params, QMap<QString, QVariant> data);
    void get(QString endpoint, QStringList params);
    void del(QString endpoint, QStringList params);
    
private:
    QNetworkAccessManager* networkManager;
    QUrl url;
    
signals:
    void resultReceived(QVariant var);
    
public slots:
    
private slots:
    void onResult(QNetworkReply *reply);
    
};

#endif // REST_ADAPTER_H
