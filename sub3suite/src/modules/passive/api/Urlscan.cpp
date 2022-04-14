#include "Urlscan.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define SEARCH_ASN 0
#define SEARCH_DOMAIN 1
#define SEARCH_IP 2
#define SEARCH_URL 3

/*
 * use for malicious url analysis...
 */
Urlscan::Urlscan(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_URLSCAN;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedSubdomain);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedAsn);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedUrl);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedIp);
}
Urlscan::~Urlscan(){
    delete manager;
}

void Urlscan::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case SEARCH_ASN:
            url.setUrl("https://urlscan.io/api/v1/search/?q=asn:AS"+target);
            break;
        case SEARCH_DOMAIN:
            url.setUrl("https://urlscan.io/api/v1/search/?q=domain:"+target);
            break;
        case SEARCH_IP:
            url.setUrl("https://urlscan.io/api/v1/search/?q=ip:"+target);
            break;
        case SEARCH_URL:
            url.setUrl("https://urlscan.io/api/v1/search/?q=url:"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        url.setUrl("https://urlscan.io/api/v1/search/?q=domain:"+target);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_IP){
        url.setUrl("https://urlscan.io/api/v1/search/?q=ip:"+target);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_ASN){
        url.setUrl("https://urlscan.io/api/v1/search/?q=asn:AS"+target);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_URL){
        url.setUrl("https://urlscan.io/api/v1/search/?q=url:"+target);
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

void Urlscan::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray resultsArray = document.object()["results"].toArray();

    foreach(const QJsonValue &value, resultsArray){
        QJsonObject page = value["page"].toObject();
        emit resultSubdomain(page["domain"].toString());
        log.resultsCount++;
        emit resultSubdomain(page["ptr"].toString());
        log.resultsCount++;
    }

    this->end(reply);
}

void Urlscan::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray resultsArray = document.object()["results"].toArray();

    foreach(const QJsonValue &value, resultsArray){
        QJsonObject page = value["page"].toObject();
        emit resultURL(page["url"].toString());
        log.resultsCount++;
    }
    this->end(reply);
}

void Urlscan::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray resultsArray = document.object()["results"].toArray();

    foreach(const QJsonValue &value, resultsArray){
        QJsonObject page = value["page"].toObject();
        emit resultASN(page["asn"].toString(), page["asnname"].toString());
        log.resultsCount++;
    }
    this->end(reply);
}

void Urlscan::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray resultsArray = document.object()["results"].toArray();

    foreach(const QJsonValue &value, resultsArray){
        emit resultIP(value["page"].toObject()["ip"].toString());
        log.resultsCount++;
    }
    this->end(reply);
}
