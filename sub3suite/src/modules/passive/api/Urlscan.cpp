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
    log.moduleName = "UrlScan";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedSubdomain);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedAsn);
    if(args.outputUrl)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedUrl);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Urlscan::replyFinishedIp);
}
Urlscan::~Urlscan(){
    delete manager;
}

void Urlscan::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        url.setUrl("https://urlscan.io/api/v1/search/?q=domain:"+target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args.inputDomain){
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
