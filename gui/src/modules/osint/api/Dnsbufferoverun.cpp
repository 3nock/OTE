#include "Dnsbufferoverun.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define SUBOMAINS 0

Dnsbufferoverun::Dnsbufferoverun(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "DnsBufferoverRun";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedSubdomain);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedSubdomainIp);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinishedIp);
}
Dnsbufferoverun::~Dnsbufferoverun(){
    delete manager;
}

void Dnsbufferoverun::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        url.setUrl("https://dns.bufferover.run/dns?q="+args->target);
    }
    if(args->inputDomain){
        url.setUrl("https://dns.bufferover.run/dns?q="+args->target);
    }

    request.setUrl(url);
    manager->get(request);
    activeRequests++;
}

void Dnsbufferoverun::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObject = document.object();

    QJsonArray FDNS_A = mainObject["FDNS_A"].toArray();
    foreach(const QJsonValue &value, FDNS_A){
        emit subdomain(value.toString().split(",")[1]); // subdomain
        log.resultsCount++;
    }
    QJsonArray RDNS = mainObject["RDNS"].toArray();
    foreach(const QJsonValue &value, RDNS){
        emit subdomain(value.toString().split(",")[1]); // subdomain
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
    QJsonObject mainObject = document.object();

    QJsonArray FDNS_A = mainObject["FDNS_A"].toArray();
    foreach(const QJsonValue &value, FDNS_A){
        emit ip(value.toString().split(",")[0]); // ip-address
        log.resultsCount++;
    }
    QJsonArray RDNS = mainObject["RDNS"].toArray();
    foreach(const QJsonValue &value, RDNS){
        emit ip(value.toString().split(",")[0]); // ip-address
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
    QJsonObject mainObject = document.object();

    QJsonArray FDNS_A = mainObject["FDNS_A"].toArray();
    foreach(const QJsonValue &value, FDNS_A){
        QStringList subdomainAndIp = value.toString().split(","); // ip-address,subdomain
        emit subdomainIp(subdomainAndIp[1], subdomainAndIp[0]);
        log.resultsCount++;
    }
    QJsonArray RDNS = mainObject["RDNS"].toArray();
    foreach(const QJsonValue &value, RDNS){
        QStringList subdomainAndIp = value.toString().split(","); // ip-address,subdomain
        emit subdomainIp(subdomainAndIp[1], subdomainAndIp[0]);
        log.resultsCount++;
    }
    end(reply);
}
