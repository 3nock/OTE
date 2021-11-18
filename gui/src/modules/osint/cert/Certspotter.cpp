#include "Certspotter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ISSUEANCES 0

/*
 * free for 100 queries an hour - keep track
 */
Certspotter::Certspotter(ScanArgs *args) : AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "CertSpotter";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Certspotter::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Certspotter::replyFinishedSubdomain);
    if(args->outputSSLCert)
        connect(manager, &MyNetworkAccessManager::finished, this, &Certspotter::replyFinishedSSLCert);
}
Certspotter::~Certspotter(){
    delete manager;
}

void Certspotter::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case ISSUEANCES:
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&include_subdomains=true&expand=cert&expand=issuer&expand=dns_names");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&include_subdomains=true&expand=dns_names");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputSSLCert){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&include_subdomains=true&expand=cert");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Certspotter::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array())
    {
        QJsonArray dns_names = value.toObject()["dns_names"].toArray();
        foreach(const QJsonValue &domain, dns_names){
            emit subdomain(domain.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void Certspotter::replyFinishedSSLCert(QNetworkReply *reply){
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
        emit sslCert(sha256);
        log.resultsCount++;
    }

    end(reply);
}
