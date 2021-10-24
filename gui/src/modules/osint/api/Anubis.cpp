#include "Anubis.h"
#include <QJsonDocument>
#include <QJsonArray>

#define SUBDOMAIN 0

Anubis::Anubis(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Anubis";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Anubis::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Anubis::replyFinishedSubdomain);
}
Anubis::~Anubis(){
    delete manager;
}

void Anubis::start(){
    QNetworkRequest request;

    if(args->raw){
        QUrl url("https://jldc.me/anubis/subdomains/"+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputDomain){
        QUrl url("https://jldc.me/anubis/subdomains/"+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Anubis::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = document.array();
        foreach(const QJsonValue &value, subdomainList){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
    }
    end(reply);
}
