#include "Dnsbufferoverun.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


#define SUBOMAINS 0


Dnsbufferoverun::Dnsbufferoverun(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = OSINT_MODULE_DNSBUFFEROVERRUN;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedSubdomain);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedSubdomainIp);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedIp);
}
Dnsbufferoverun::~Dnsbufferoverun(){
    delete manager;
}

void Dnsbufferoverun::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case SUBOMAINS:
            url.setUrl("https://dns.bufferover.run/dns?q="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomainIp || args.outputSubdomain || args.outputIp){
            url.setUrl("https://dns.bufferover.run/dns?q="+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Dnsbufferoverun::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    /* for forward dns */
    foreach(const QJsonValue &value, mainObj["FDNS_A"].toArray()){
        emit resultSubdomain(value.toString().split(",")[1]);
        log.resultsCount++;
    }
    /* for reverse dns */
    foreach(const QJsonValue &value, mainObj["RDNS"].toArray()){
        emit resultSubdomain(value.toString().split(",")[1]);
        log.resultsCount++;
    }

    end(reply);
}

void Dnsbufferoverun::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    /* for forward dns */
    foreach(const QJsonValue &value, mainObj["FDNS_A"].toArray()){
        emit resultIp(value.toString().split(",")[0]);
        log.resultsCount++;
    }
    /* for reverse dns */
    foreach(const QJsonValue &value, mainObj["RDNS"].toArray()){
        emit resultIp(value.toString().split(",")[0]);
        log.resultsCount++;
    }

    end(reply);
}

void Dnsbufferoverun::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    /* for forward dns */
    foreach(const QJsonValue &value, mainObj["FDNS_A"].toArray()){
        QStringList subdomainAndIp = value.toString().split(","); // ip-address,subdomain
        emit resultSubdomainIp(subdomainAndIp[1], subdomainAndIp[0]);
        log.resultsCount++;
    }
    /* for reverse dns */
    foreach(const QJsonValue &value, mainObj["RDNS"].toArray()){
        QStringList subdomainAndIp = value.toString().split(","); // ip-address,subdomain
        emit resultSubdomainIp(subdomainAndIp[1], subdomainAndIp[0]);
        log.resultsCount++;
    }

    end(reply);
}
