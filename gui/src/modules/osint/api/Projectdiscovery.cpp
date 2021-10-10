#include "Projectdiscovery.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Projectdiscovery::Projectdiscovery(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Projectdiscovery::replyFinished);
}
Projectdiscovery::~Projectdiscovery(){
    delete manager;
}

void Projectdiscovery::start(){
    QNetworkRequest request;
    QUrl url("https://dns.projectdiscovery.io/dns/"+args->target+"/subdomains");
    request.setUrl(url);
    request.setRawHeader("Authorization", "key");
    manager->get(request);
}

void Projectdiscovery::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        QString error = jsonObject["error"].toString();
        if(error.isNull() || error.isEmpty()){
            QJsonArray subdomainList = jsonObject["subdomains"].toArray();
            foreach(const QJsonValue &value, subdomainList)
                emit scanResults(value.toString());
        }
        else{
            emit scanResults(error);
        }
    }
    else
    {
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}