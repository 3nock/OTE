#include "Shodan.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ACCOUNT_PROFILE 0
#define BULK_DATA 1
#define DNS_DOMAIN 2
#define DNS_RESOLVE 3
#define DNS_REVERSE 4
#define HOST_COUNT 5
#define HOST_IP 6
#define HOST_SEARCH 7
#define HOST_SEARCH_FACETS 8
#define HOST_SEARCH_FILTERS 9
#define HOST_SEARCH_TOKENS 10


/* 1 request per second */
Shodan::Shodan(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_SHODAN;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Shodan::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Shodan::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Shodan::replyFinishedIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Shodan::replyFinishedAsn);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Shodan::replyFinishedSubdomainIp);

    /* getting api-key */
    m_key = APIKEY.value(OSINT_MODULE_SHODAN).toString();
}
Shodan::~Shodan(){
    delete manager;
}

void Shodan::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption){
        case HOST_IP:
            url.setUrl("https://api.shodan.io/shodan/host/"+target+"?key="+m_key);
            break;
        case HOST_COUNT:
            url.setUrl("https://api.shodan.io/shodan/host/count?key="+m_key+"&query="+target+"&facets=org,os");
            break;
        case HOST_SEARCH:
            url.setUrl("https://api.shodan.io/shodan/host/search?key="+m_key+"&query="+target+"&facets=country");
            break;
        case HOST_SEARCH_FACETS:
            url.setUrl("https://api.shodan.io/shodan/host/search/facets?key="+m_key);
            break;
        case HOST_SEARCH_FILTERS:
            url.setUrl("https://api.shodan.io/shodan/host/search/filters?key="+m_key);
            break;
        case HOST_SEARCH_TOKENS:
            url.setUrl("https://api.shodan.io/shodan/host/search/tokens?key="+m_key+"&query="+target);
            break;
        case BULK_DATA:
            url.setUrl("https://api.shodan.io/shodan/data/"+target+"?key="+m_key);
            break;
        case ACCOUNT_PROFILE:
            url.setUrl("https://api.shodan.io/account/profile?key="+m_key);
            break;
        case DNS_DOMAIN:
            url.setUrl("https://api.shodan.io/dns/domain/"+target+"?key="+m_key);
            break;
        case DNS_RESOLVE:
            url.setUrl("https://api.shodan.io/dns/resolve?hostnames="+target+"&key="+m_key);
            break;
        case DNS_REVERSE:
            url.setUrl("https://api.shodan.io/dns/reverse?ips="+target+"&key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomain || args.outputSubdomainIp || args.outputIp){
            url.setUrl("https://api.shodan.io/dns/domain/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, DNS_DOMAIN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputIp){
        if(args.outputAsn || args.outputSubdomain){
            url.setUrl("https://api.shodan.io/shodan/host/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, HOST_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Shodan::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DNS_DOMAIN:
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            if(type == "A" || type == "AAAA"){
                QString subdomain = value.toObject()["subdomain"].toString();
                QString address = value.toObject()["value"].toString();
                emit resultSubdomainIp(subdomain, address);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Shodan::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DNS_DOMAIN:
    {
        /* direct subdomains */
        QJsonArray subdomains = document.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomains){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
        /* dns records */
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            QString hostname = value.toObject()["value"].toString();
            if(type == "MX"){
                emit resultMX(hostname);
                log.resultsCount++;
            }
            if(type == "NS"){
                emit resultNS(hostname);
                log.resultsCount++;
            }
            if(type == "CNAME"){
                emit resultCNAME(hostname);
                log.resultsCount++;
            }
            if(type == "TXT"){
                emit resultTXT(hostname);
                log.resultsCount++;
            }
        }
    }
        break;

    case HOST_IP:
        foreach(const QJsonValue &dataValue, data){
            foreach(const QJsonValue &value, dataValue.toObject()["hostnames"].toArray()){
                emit resultSubdomain(value.toString());
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}

void Shodan::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DNS_DOMAIN:
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            if(type == "A"){
                QString address = value.toObject()["value"].toString();
                emit resultA(address);
                log.resultsCount++;
            }
            if(type == "AAAA"){
                QString address = value.toObject()["value"].toString();
                emit resultAAAA(address);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void Shodan::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOST_IP:
        foreach(const QJsonValue &value, data){
            emit resultASN(value.toObject()["asn"].toString(), value.toObject()["org"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
