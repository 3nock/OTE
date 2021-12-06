#include "UKWebArchive.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define URL 0

/*
 * returns results in ndjson formart...
 * for now only one type of query...
 * needs more fixing...
 */
UKWebArchive::UKWebArchive(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "UKWebArchive";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &UKWebArchive::replyFinishedRawNdjson);
    if(args->outputUrl)
        connect(manager, &NetworkAccessManager::finished, this, &UKWebArchive::replyFinishedUrl);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &UKWebArchive::replyFinishedSubdomain);
}
UKWebArchive::~UKWebArchive(){
    delete manager;
}

void UKWebArchive::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case URL:
            url.setUrl("https://www.webarchive.org.uk/wayback/archive/cdx?matchType=domain&output=json&url="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputUrl || args->outputSubdomain){
            url.setUrl("https://www.webarchive.org.uk/wayback/archive/cdx?matchType=domain&output=json&url="+args->target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void UKWebArchive::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /* converting ndjson to json array document */
    QByteArray byteDocument = reply->readAll();
    byteDocument = byteDocument.simplified();
    byteDocument.replace("\r\n", ",");
    byteDocument.push_back("]");
    byteDocument.push_front("[");

    /* ... */
    QJsonDocument document = QJsonDocument::fromJson(byteDocument);
    QJsonArray urls = document.array();

    foreach(const QJsonValue &urlValue, urls){
        QString domainUrl = urlValue.toObject()["url"].toString();
        emit url(domainUrl);
        log.resultsCount++;
    }

    end(reply);
}

void UKWebArchive::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /* converting ndjson to json array document */
    QByteArray byteDocument = reply->readAll();
    byteDocument = byteDocument.simplified();
    byteDocument.replace("\r\n", ",");
    byteDocument.push_back("]");
    byteDocument.push_front("[");

    /* ... */
    QJsonDocument document = QJsonDocument::fromJson(byteDocument);
    QJsonArray urls = document.array();

    foreach(const QJsonValue &urlValue, urls){
        /* getting url */
        QString domainUrl = urlValue.toObject()["url"].toString();

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
