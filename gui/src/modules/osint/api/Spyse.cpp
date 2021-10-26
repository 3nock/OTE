#include "Spyse.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define AS 0
#define CVE 1
#define EMAILS 2
#define DNS_HISTORY 3
#define DOMAINS 4
#define IPV4 5
#define IPV6 6
#define SSL_CERT 7


Spyse::Spyse(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Spyse";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Spyse::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Spyse::replyFinishedSubdomain);
    if(args->emails)
        connect(manager, &MyNetworkAccessManager::finished, this, &Spyse::replyFinishedEmail);
}
Spyse::~Spyse(){
    delete manager;
}

void Spyse::start(){
    QNetworkRequest request;
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0");

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case DOMAINS:
            url.setUrl("https://api.spyse.com/v4/data/domain/"+args->target);
            break;
        case IPV4:
            url.setUrl("https://api.spyse.com/v4/data/ip/"+args->target);
            break;
        case SSL_CERT:
            url.setUrl("https://api.spyse.com/v4/data/certificate/"+args->target);
            break;
        case AS:
            url.setUrl("https://api.spyse.com/v4/data/as/"+args->target);
            break;
        case CVE:
            url.setUrl("https://api.spyse.com/v4/data/cve/"+args->target);
            break;
        case EMAILS:
            url.setUrl("https://api.spyse.com/v4/data/email/"+args->target);
            break;
        case DNS_HISTORY:
            url.setUrl("https://api.spyse.com/v4/data/history/dns/ANY/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.spyse.com/v3/data/domain/subdomain?limit=100&domain="+args->target);
            request.setAttribute(QNetworkRequest::User, DOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Spyse::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject dataObject = document.object()["data"].toObject();

    QJsonArray items = dataObject["items"].toArray();
    foreach(const QJsonValue &value, items){
        emit subdomain(value["email"].toString());
        log.resultsCount++;
    }
    end(reply);
}

void Spyse::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject dataObject = document.object()["data"].toObject();
    QJsonArray items = dataObject["items"].toArray();

    foreach(const QJsonValue &value, items){
        emit subdomain(value["name"].toString());
        log.resultsCount++;
    }
    end(reply);
}
