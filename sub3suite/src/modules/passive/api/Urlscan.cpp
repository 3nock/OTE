#include "Urlscan.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*
 * does not produce clean results yet...
 * use for malicious url analysis...
 */
Urlscan::Urlscan(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
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
        url.setUrl("https://urlscan.io/api/v1/search/?q=domain:"+target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args.input_Domain){
        url.setUrl("https://urlscan.io/api/v1/search/?q=domain:"+target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
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
    }
    end(reply);
}

void Urlscan::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray resultsArray = document.object()["results"].toArray();

    foreach(const QJsonValue &value, resultsArray){
        QJsonObject page = value["page"].toObject();
        emit resultIP(page["ptr"].toString());
        log.resultsCount++;
    }
    end(reply);
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
    end(reply);
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
        emit resultASN(page["asn"].toString(), "");
        log.resultsCount++;
    }
    end(reply);
}
