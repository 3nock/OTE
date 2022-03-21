#include "CertspotterFree.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ISSUEANCES 0

/*
 * free for 100 queries an hour - keep track
 */
CertspotterFree::CertspotterFree(ScanArgs args) : AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "CertspotterFree";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CertspotterFree::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CertspotterFree::replyFinishedSubdomain);
    if(args.outputSSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CertspotterFree::replyFinishedSSL);
}
CertspotterFree::~CertspotterFree(){
    delete manager;
}

void CertspotterFree::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ISSUEANCES:
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+target+"&include_subdomains=true&expand=cert&expand=issuer&expand=dns_names");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomain){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+target+"&include_subdomains=true&expand=dns_names");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.outputSSL){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+target+"&include_subdomains=true&expand=cert");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void CertspotterFree::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array())
    {
        QJsonArray dns_names = value.toObject()["dns_names"].toArray();
        foreach(const QJsonValue &domain, dns_names){
            emit resultSubdomain(domain.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void CertspotterFree::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array())
    {
        QJsonObject cert = value.toObject()["cert"].toObject();
        QString sha256 = cert["sha256"].toString();
        /* ... */
        emit resultSSL(sha256);
        log.resultsCount++;
    }

    end(reply);
}
