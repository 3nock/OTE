#include "NetworksDB.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define AS_INFO 0
#define AS_NETWORKS 1
#define DOMAINS_IN_NETWORK 2
#define DOMAINS_ON_IP 3
#define IP_GEOLOCATION 4
#define IP_INFO 5
#define IPS_FOR_DOMAIN 6
#define ORG_INFO 7
#define ORG_NETWORK 8
#define ORG_SEARCH 9

/*
 * 1000 queries per month for free account...
 */
NetworksDB::NetworksDB(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "NetworksDB";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &NetworksDB::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &NetworksDB::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &NetworksDB::replyFinishedIp);
    if(args->outputCidr)
        connect(manager, &MyNetworkAccessManager::finished, this, &NetworksDB::replyFinishedPrefixes);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &NetworksDB::replyFinishedSubdomainIp);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("networksdb").toString();
    Config::generalConfig().endGroup();
}
NetworksDB::~NetworksDB(){
    delete manager;
}

void NetworksDB::start(){
    QNetworkRequest request;
    request.setRawHeader("X-Api-Key", m_key.toUtf8());

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case AS_INFO:
            url.setUrl("https://networksdb.io/api/asn-info?asn="+args->target);
            break;
        case AS_NETWORKS:
            url.setUrl("https://networksdb.io/api/asn-networks?asn="+args->target);
            break;
        case DOMAINS_IN_NETWORK:
            url.setUrl("https://networksdb.io/api/mass-reverse-dns?cidr="+args->target);
            break;
        case DOMAINS_ON_IP:
            url.setUrl("https://networksdb.io/api/reverse-dns?ip="+args->target);
            break;
        case IP_GEOLOCATION:
            url.setUrl("https://networksdb.io/api/ip-geo?ip="+args->target);
            break;
        case IP_INFO:
            url.setUrl("https://networksdb.io/api/ip-info?ip="+args->target);
            break;
        case IPS_FOR_DOMAIN:
            url.setUrl("https://networksdb.io/api/dns?domain="+args->target);
            break;
        case ORG_INFO:
            url.setUrl("https://networksdb.io/api/org-info?id="+args->target);
            break;
        case ORG_NETWORK:
            url.setUrl("https://networksdb.io/api/org-networks?id="+args->target);
            break;
        case ORG_SEARCH:
            url.setUrl("https://networksdb.io/api/org-search?search="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputIp){
        if(args->outputSubdomain){
            url.setUrl("https://networksdb.io/api/reverse-dns?ip="+args->target);
            request.setAttribute(QNetworkRequest::User, DOMAINS_ON_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputDomain){
        if(args->outputIp){
            url.setUrl("https://networksdb.io/api/dns?domain="+args->target);
            request.setAttribute(QNetworkRequest::User, IPS_FOR_DOMAIN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputAsn){
        if(args->outputCidr){
            url.setUrl("https://networksdb.io/api/asn-networks?asn="+args->target);
            request.setAttribute(QNetworkRequest::User, AS_NETWORKS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputCidr){
        if(args->outputSubdomain || args->outputIp || args->outputSubdomainIp){
            url.setUrl("https://networksdb.io/api/mass-reverse-dns?cidr="+args->target);
            request.setAttribute(QNetworkRequest::User, DOMAINS_IN_NETWORK);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void NetworksDB::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == DOMAINS_IN_NETWORK){
        foreach(const QJsonValue &value, results){
            QJsonArray domains = value.toObject()["domains"].toArray();
            QString address = value.toObject()["ip"].toString();
            foreach(const QJsonValue &domain, domains){
                emit subdomainIp(domain.toString(), address);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void NetworksDB::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == DOMAINS_IN_NETWORK){
        foreach(const QJsonValue &value, results){
            QJsonArray domains = value.toObject()["domains"].toArray();
            foreach(const QJsonValue &domain, domains){
                emit subdomain(domain.toString());
                log.resultsCount++;
            }
        }
    }

    if(requestType == DOMAINS_ON_IP){
        foreach(const QJsonValue &value, results){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void NetworksDB::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == DOMAINS_IN_NETWORK){
        foreach(const QJsonValue &value, results){
            QString address = value.toObject()["ip"].toString();
            emit ip(address);
            log.resultsCount++;
        }
    }

    if(requestType == IPS_FOR_DOMAIN){
        foreach(const QJsonValue &value, results){
            emit ip(value.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void NetworksDB::replyFinishedPrefixes(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == IPS_FOR_DOMAIN){
        foreach(const QJsonValue &value, results){
            QString cidr = value.toObject()["cidr"].toString();
            QString org = value.toObject()["organisation"].toObject()["name"].toString();

            emit prefix(cidr, org);
            log.resultsCount++;
        }
    }

    end(reply);
}
