#include "Arquivo.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define CDX_SERVER 0
#define TEXTSEARCH 1

/*
 * for now use *. hence data on subdomains
 * cdx uses ndjson...
 * maxItems set to 100...
 */
Arquivo::Arquivo(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Arquivo";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Arquivo::replyFinishedRawJson);
    if(args->outputUrl)
        connect(manager, &NetworkAccessManager::finished, this, &Arquivo::replyFinishedUrl);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Arquivo::replyFinishedSubdomain);
}
Arquivo::~Arquivo(){
    delete manager;
}

void Arquivo::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case CDX_SERVER:
            url.setUrl("https://arquivo.pt/wayback/cdx?url=*."+args->target+"&output=json");
            break;
        case TEXTSEARCH:
            url.setUrl("https://arquivo.pt/textsearch?q=*."+args->target+"&prettyPrint=false&maxItems=100");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputUrl || args->outputSubdomain){
            url.setUrl("https://arquivo.pt/textsearch?q=*."+args->target+"&prettyPrint=false&maxItems=100");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Arquivo::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray response_items = document.object()["response_items"].toArray();

    foreach(const QJsonValue &response_item, response_items){
        QString urlValue = response_item.toObject()["originalURL"].toString();
        emit url(urlValue);
        log.resultsCount++;
    }

    end(reply);
}

void Arquivo::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray response_items = document.object()["response_items"].toArray();

    foreach(const QJsonValue &response_item, response_items)
    {
        /* getting url */
        QString domainUrl = response_item.toObject()["originalURL"].toString();

        /* extracting subdomain from url...*/
        domainUrl.remove("http://");
        domainUrl.remove("https://");
        domainUrl = domainUrl.split("/").at(0);

        /*  emiting subdomain... */
        emit subdomain(domainUrl);
        log.resultsCount++;
    }

    end(reply);
}
