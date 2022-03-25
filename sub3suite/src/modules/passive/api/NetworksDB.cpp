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
NetworksDB::NetworksDB(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_NETWORKSDB;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &NetworksDB::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &NetworksDB::replyFinishedSubdomain);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &NetworksDB::replyFinishedIp);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &NetworksDB::replyFinishedCidr);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &NetworksDB::replyFinishedAsn);
    if(args.output_HostnameIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &NetworksDB::replyFinishedSubdomainIp);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_NETWORKSDB).toString();
}
NetworksDB::~NetworksDB(){
    delete manager;
}

void NetworksDB::start(){
    QNetworkRequest request;
    request.setRawHeader("X-Api-Key", m_key.toUtf8());

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case AS_INFO:
            url.setUrl("https://networksdb.io/api/asn-info?asn="+target);
            break;
        case AS_NETWORKS:
            url.setUrl("https://networksdb.io/api/asn-networks?asn="+target);
            break;
        case DOMAINS_IN_NETWORK:
            url.setUrl("https://networksdb.io/api/mass-reverse-dns?cidr="+target);
            break;
        case DOMAINS_ON_IP:
            url.setUrl("https://networksdb.io/api/reverse-dns?ip="+target);
            break;
        case IP_GEOLOCATION:
            url.setUrl("https://networksdb.io/api/ip-geo?ip="+target);
            break;
        case IP_INFO:
            url.setUrl("https://networksdb.io/api/ip-info?ip="+target);
            break;
        case IPS_FOR_DOMAIN:
            url.setUrl("https://networksdb.io/api/dns?domain="+target);
            break;
        case ORG_INFO:
            url.setUrl("https://networksdb.io/api/org-info?id="+target);
            break;
        case ORG_NETWORK:
            url.setUrl("https://networksdb.io/api/org-networks?id="+target);
            break;
        case ORG_SEARCH:
            url.setUrl("https://networksdb.io/api/org-search?search="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.input_IP){
        if(args.output_Hostname){
            url.setUrl("https://networksdb.io/api/reverse-dns?ip="+target);
            request.setAttribute(QNetworkRequest::User, DOMAINS_ON_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.output_CIDR){
            url.setUrl("https://networksdb.io/api/ip-info?ip="+target);
            request.setAttribute(QNetworkRequest::User, IP_INFO);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_Domain){
        if(args.output_IP){
            url.setUrl("https://networksdb.io/api/dns?domain="+target);
            request.setAttribute(QNetworkRequest::User, IPS_FOR_DOMAIN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_ASN){
        if(args.output_CIDR){
            url.setUrl("https://networksdb.io/api/asn-networks?asn="+target);
            request.setAttribute(QNetworkRequest::User, AS_NETWORKS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_CIDR){
        if(args.output_Hostname || args.output_IP || args.output_HostnameIP){
            url.setUrl("https://networksdb.io/api/mass-reverse-dns?cidr="+target);
            request.setAttribute(QNetworkRequest::User, DOMAINS_IN_NETWORK);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_Search){
        if(args.output_ASN){
            url.setUrl("https://networksdb.io/api/org-search?search="+target);
            request.setAttribute(QNetworkRequest::User, ORG_SEARCH);
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS_IN_NETWORK:
        foreach(const QJsonValue &value, results){
            QJsonArray domains = value.toObject()["domains"].toArray();
            QString address = value.toObject()["ip"].toString();
            foreach(const QJsonValue &domain, domains){
                emit resultSubdomainIp(domain.toString(), address);
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS_IN_NETWORK:
        foreach(const QJsonValue &value, results){
            QJsonArray domains = value.toObject()["domains"].toArray();
            foreach(const QJsonValue &domain, domains){
                emit resultSubdomain(domain.toString());
                log.resultsCount++;
            }
        }
        break;

    case DOMAINS_ON_IP:
        foreach(const QJsonValue &value, results){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
        break;
    }

    end(reply);
}

void NetworksDB::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS_IN_NETWORK:
        foreach(const QJsonValue &value, results){
            QString address = value.toObject()["ip"].toString();
            emit resultIP(address);
            log.resultsCount++;
        }
        break;

    case IPS_FOR_DOMAIN:
        foreach(const QJsonValue &value, results){
            emit resultIP(value.toString());
            log.resultsCount++;
        }
        break;
    }

    end(reply);
}

void NetworksDB::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case AS_NETWORKS:
    case IPS_FOR_DOMAIN:
        foreach(const QJsonValue &value, results){
            emit resultCIDR(value.toObject()["cidr"].toString());
            log.resultsCount++;
        }
        break;

    case IP_INFO:
        QString cidr = document.object()["network"].toObject()["cidr"].toString();
        emit resultCIDR(cidr);
        log.resultsCount++;
        break;
    }

    end(reply);
}

void NetworksDB::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ORG_SEARCH:
        foreach(const QJsonValue &value, results){
            QString organisation = value.toObject()["organisation"].toString();
            QJsonArray asns = value.toObject()["asns"].toArray();
            foreach(const QJsonValue &asn, asns){
                emit resultASN(asn.toString(), organisation);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}
