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
UKWebArchive::UKWebArchive(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_UKWEBARCHIVE;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &UKWebArchive::replyFinishedRawNdjson);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &UKWebArchive::replyFinishedUrl);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &UKWebArchive::replyFinishedSubdomain);
}
UKWebArchive::~UKWebArchive(){
    delete manager;
}

void UKWebArchive::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case URL:
            url.setUrl("https://www.webarchive.org.uk/wayback/archive/cdx?matchType=domain&output=json&url="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_URL || args.output_Hostname){
            url.setUrl("https://www.webarchive.org.uk/wayback/archive/cdx?matchType=domain&output=json&url="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_URL){
        if(args.output_URL || args.output_Hostname){
            url.setUrl("https://www.webarchive.org.uk/wayback/archive/cdx?matchType=domain&output=json&url="+target);
            request.setUrl(url);
            manager->get(request);
            return;
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

    QJsonDocument document = QJsonDocument::fromJson(byteDocument);
    QJsonArray urls = document.array();

    foreach(const QJsonValue &urlValue, urls){
        QString domainUrl = urlValue.toObject()["url"].toString();
        emit resultURL(domainUrl);
        log.resultsCount++;
    }

    this->end(reply);
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
        emit resultSubdomain(domainUrl);
        log.resultsCount++;
    }

    this->end(reply);
}
