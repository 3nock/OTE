#include "Projectdiscovery.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Projectdiscovery::Projectdiscovery(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Projectdiscovery::replyFinished);
}
Projectdiscovery::~Projectdiscovery(){
    delete manager;
}

void Projectdiscovery::start(){
    QNetworkRequest request;
    QUrl url("https://dns.projectdiscovery.io/dns/"+target+"/subdomains");
    request.setUrl(url);
    request.setRawHeader("Authorization", "key");
    manager->get(request);
}

void Projectdiscovery::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        // an error occured...
    }
    else
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        QJsonArray subdomainList = jsonObject["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
    }
    reply->deleteLater();
    emit quitThread();
}
