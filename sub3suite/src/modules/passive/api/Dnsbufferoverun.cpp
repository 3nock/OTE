#include "Dnsbufferoverun.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


#define SUBOMAINS 0


Dnsbufferoverun::Dnsbufferoverun(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_DNSBUFFEROVERRUN;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedSubdomain);
    if(args.output_HostnameIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedSubdomainIp);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedIp);
}
Dnsbufferoverun::~Dnsbufferoverun(){
    delete manager;
}

void Dnsbufferoverun::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case SUBOMAINS:
            url.setUrl("https://dns.bufferover.run/dns?q="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_HostnameIP || args.output_Hostname || args.output_IP){
            url.setUrl("https://dns.bufferover.run/dns?q="+target);
            request.setUrl(url);
            manager->get(request);
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

    this->end(reply);
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
        emit resultIP(value.toString().split(",")[0]);
        log.resultsCount++;
    }
    /* for reverse dns */
    foreach(const QJsonValue &value, mainObj["RDNS"].toArray()){
        emit resultIP(value.toString().split(",")[0]);
        log.resultsCount++;
    }

    this->end(reply);
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

    this->end(reply);
}
