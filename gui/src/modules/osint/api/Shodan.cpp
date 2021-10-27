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


/* 1 request/ second */
/*
 * able to reverse ip addresses to domain names, targets are comma seperated...
 * also able to resolve domains, targets are comma seperated...
 */
Shodan::Shodan(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Shodan";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Shodan::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Shodan::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Shodan::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &Shodan::replyFinishedAsn);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Shodan::replyFinishedSubdomainIp);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("shodan").toString();
    Config::generalConfig().endGroup();
}
Shodan::~Shodan(){
    delete manager;
}

void Shodan::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption){
        case HOST_IP:
            url.setUrl("https://api.shodan.io/shodan/host/"+args->target+"?key="+m_key);
            break;
        case HOST_COUNT:
            url.setUrl("https://api.shodan.io/shodan/host/count?key="+m_key+"&query="+args->target+"&facets=org,os");
            break;
        case HOST_SEARCH:
            url.setUrl("https://api.shodan.io/shodan/host/search?key="+m_key+"&query="+args->target+"&facets=country");
            break;
        case HOST_SEARCH_FACETS:
            url.setUrl("https://api.shodan.io/shodan/host/search/facets?key="+m_key);
            break;
        case HOST_SEARCH_FILTERS:
            url.setUrl("https://api.shodan.io/shodan/host/search/filters?key="+m_key);
            break;
        case HOST_SEARCH_TOKENS:
            url.setUrl("https://api.shodan.io/shodan/host/search/tokens?key="+m_key+"&query="+args->target);
            break;
        case BULK_DATA:
            url.setUrl("https://api.shodan.io/shodan/data/"+args->target+"?key="+m_key);
            break;
        case ACCOUNT_PROFILE:
            url.setUrl("https://api.shodan.io/account/profile?key="+m_key);
            break;
        case DNS_DOMAIN:
            url.setUrl("https://api.shodan.io/dns/domain/"+args->target+"?key="+m_key);
            break;
        case DNS_RESOLVE:
            url.setUrl("https://api.shodan.io/dns/resolve?hostnames="+args->target+"&key="+m_key);
            break;
        case DNS_REVERSE:
            url.setUrl("https://api.shodan.io/dns/reverse?ips="+args->target+"&key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://api.shodan.io/dns/domain/"+args->target+"?key="+m_key);
        request.setAttribute(QNetworkRequest::User, DNS_DOMAIN);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputIp){
        url.setUrl("https://api.shodan.io/shodan/host/"+args->target+"?key="+m_key);
        request.setAttribute(QNetworkRequest::User, HOST_IP);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Shodan::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200){
        emit errorLog("Error occurred!");
        end(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == DNS_DOMAIN){
        /* dns records */
        QJsonArray data = document.object()["data"].toArray();
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            if(type == "A" || type == "AAAA"){
                QString subdomain = value.toObject()["subdomain"].toString();
                QString address = value.toObject()["value"].toString();
                emit subdomainIp(subdomain, address);
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
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200){
        emit errorLog("Error occurred!");
        end(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == DNS_DOMAIN){
        /* direct subdomains */
        QJsonArray subdomains = document.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomains){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
        /* dns records */
        QJsonArray data = document.object()["data"].toArray();
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            QString hostname = value.toObject()["value"].toString();
            if(type == "MX"){
                emit MX(hostname);
                log.resultsCount++;
            }
            if(type == "NS"){
                emit NS(hostname);
                log.resultsCount++;
            }
            if(type == "CNAME"){
                emit CNAME(hostname);
                log.resultsCount++;
            }
            if(type == "TXT"){
                emit TXT(hostname);
                log.resultsCount++;
            }
        }
    }

    if(requestType == HOST_IP){
        QJsonArray data = document.object()["data"].toArray();
        foreach(const QJsonValue &dataValue, data){
            foreach(const QJsonValue &value, dataValue.toObject()["hostnames"].toArray()){
                emit subdomain(value.toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Shodan::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200){
        emit errorLog("Error occurred!");
        end(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == DNS_DOMAIN){
        /* dns records */
        QJsonArray data = document.object()["data"].toArray();
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            if(type == "A"){
                QString address = value.toObject()["value"].toString();
                emit ipA(address);
                log.resultsCount++;
            }
            if(type == "AAAA"){
                QString address = value.toObject()["value"].toString();
                emit ipAAAA(address);
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
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200){
        emit errorLog("Error occurred!");
        end(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == HOST_IP){
        QJsonArray data = document.object()["data"].toArray();
        foreach(const QJsonValue &value, data){
            emit asn(value.toObject()["asn"].toString(), "");
            log.resultsCount++;
        }
    }
    end(reply);
}
