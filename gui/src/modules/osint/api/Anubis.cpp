#include "Anubis.h"
#include <QJsonDocument>
#include <QJsonArray>


Anubis::Anubis(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Anubis";
}
Anubis::~Anubis(){
    delete manager;
}

void Anubis::start(){
    QNetworkRequest request;

    if(args->raw){
        QUrl url("https://jldc.me/anubis/subdomains/"+args->target);
        request.setUrl(url);
        connect(manager, &MyNetworkAccessManager::finished, this, &Anubis::replyFinishedRaw);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputDomain){
        QUrl url("https://jldc.me/anubis/subdomains/"+args->target);
        request.setUrl(url);
        connect(manager, &MyNetworkAccessManager::finished, this, &Anubis::replyFinishedSubdomain);
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
