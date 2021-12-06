#include "Anubis.h"
#include <QJsonDocument>
#include <QJsonArray>

#define SUBDOMAIN 0

Anubis::Anubis(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Anubis";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Anubis::replyFinishedRawJson);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Anubis::replyFinishedSubdomain);
}
Anubis::~Anubis(){
    delete manager;
}

void Anubis::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case SUBDOMAIN:
            url.setUrl("https://jldc.me/anubis/subdomains/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://jldc.me/anubis/subdomains/"+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Anubis::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray subdomainList = document.array();

    foreach(const QJsonValue &value, subdomainList){
        emit subdomain(value.toString());
        log.resultsCount++;
    }
    end(reply);
}
