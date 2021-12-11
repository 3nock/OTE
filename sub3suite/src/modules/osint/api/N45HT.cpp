#include "N45HT.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define SUBDOMAIN 0

/*
 * havent seen the api documentation yet...
 */
N45HT::N45HT(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "N45HT";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &N45HT::replyFinishedRawJson);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &N45HT::replyFinishedSubdomain);
}
N45HT::~N45HT(){
    delete manager;
}

void N45HT::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case SUBDOMAIN:
            url.setUrl("https://api.n45ht.or.id/v1/subdomain-enumeration?domain="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://api.n45ht.or.id/v1/subdomain-enumeration?domain="+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void N45HT::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray subdomains = document.object()["subdomains"].toArray();

    foreach(const QJsonValue &value, subdomains){
        emit subdomain(value.toString());
        log.resultsCount++;
    }

    end(reply);
}
