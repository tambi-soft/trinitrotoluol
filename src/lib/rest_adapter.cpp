#include "rest_adapter.h"

RESTAdapter::RESTAdapter(QObject *parent)
    : QObject(parent)
    , networkManager (new QNetworkAccessManager(this))
{
    if (QSysInfo::kernelType() == "linux")
    {
        this->url.setScheme("http");
        this->url.setHost("localhost");
        this->url.setPath("/marbox/api/");
    }
    else if (QSysInfo::kernelType() == "winnt")
    {
        this->url.setScheme("http");
        this->url.setHost("marbox.de");
        this->url.setPath("/api/");
    }
}

void RESTAdapter::post(QString endpoint, QStringList params, QMap<QString, QVariant> data)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QString endpoint_with_params = endpoint;
    foreach(QString param, params)
    {
        endpoint_with_params += "/" + param;
    }
    this->url.setPath(this->url.path() + endpoint_with_params);
    request.setUrl(this->url);
    
    QJsonDocument doc = QJsonDocument::fromVariant(data);
    
    networkManager->post(request, doc.toJson());
}

void RESTAdapter::put(QString endpoint, QStringList params, QMap<QString, QVariant> data)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QString endpoint_with_params = endpoint;
    foreach(QString param, params)
    {
        endpoint_with_params += "/" + param;
    }
    this->url.setPath(this->url.path() + endpoint_with_params);
    request.setUrl(this->url);
    
    QJsonDocument doc = QJsonDocument::fromVariant(data);
    
    networkManager->put(request, doc.toJson());
}

void RESTAdapter::get(QString endpoint, QStringList params)
{
    QNetworkRequest request;
    QString endpoint_with_params = endpoint;
    foreach(QString param, params)
    {
        endpoint_with_params += "/" + param;
    }
    this->url.setPath(this->url.path() + endpoint_with_params);
    request.setUrl(this->url);
    
    networkManager->get(request);
    
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
    
}
void RESTAdapter::onResult(QNetworkReply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    
    emit resultReceived(doc.toVariant());
}

void RESTAdapter::del(QString endpoint, QStringList params)
{
    QNetworkRequest request;
    QString endpoint_with_params = endpoint;
    foreach(QString param, params)
    {
        endpoint_with_params += "/" + param;
    }
    this->url.setPath(this->url.path() + endpoint_with_params);
    request.setUrl(this->url);
    
    networkManager->deleteResource(request);
}
