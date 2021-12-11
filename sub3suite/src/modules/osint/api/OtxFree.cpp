#include "OtxFree.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define INDICATOR_DOMAIN_GENERAL 0
#define INDICATOR_DOMAIN_GEO 1
#define INDICATOR_DOMAIN_HTTPSCANS 2
#define INDICATOR_DOMAIN_MALWARE 3
#define INDICATOR_DOMAIN_PASSIVEDNS 4
#define INDICATOR_DOMAIN_URLLIST 5
#define INDICATOR_DOMAIN_WHOIS 6
#define INDICATOR_HOSTNAME_GENERAL 7
#define INDICATOR_HOSTNAME_GEO 8
#define INDICATOR_HOSTNAME_HTTPSCANS 9
#define INDICATOR_HOSTNAME_MALWARE 10
#define INDICATOR_HOSTNAME_PASSIVEDNS 11
#define INDICATOR_HOSTNAME_URLLIST 12
#define INDICATOR_IPV4_GENERAL 13
#define INDICATOR_IPV4_GEO 14
#define INDICATOR_IPV4_MALWARE 15
#define INDICATOR_IPV4_PASSIVEDNS 16
#define INDICATOR_IPV4_REPUTATION 17
#define INDICATOR_IPV4_URLLIST 18
#define INDICATOR_IPV6_GENERAL 19
#define INDICATOR_IPV6_GEO 20
#define INDICATOR_IPV6_MALWARE 21
#define INDICATOR_IPV6_PASSIVEDNS 22
#define INDICATOR_IPV6_REPUTATION 23
#define INDICATOR_IPV6_URLLIST 24
#define INDICATOR_URL_GENERAL 25
#define INDICATOR_URL_URLLIST 26


/* 1k per hour unauthenticated, and 10k authed*/
OtxFree::OtxFree(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "otx";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &OtxFree::replyFinishedRawJson);
    if(args->outputSubdomainIp)
        connect(manager, &NetworkAccessManager::finished, this, &OtxFree::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &OtxFree::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &NetworkAccessManager::finished, this, &OtxFree::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &NetworkAccessManager::finished, this, &OtxFree::replyFinishedAsn);
}
OtxFree::~OtxFree(){
    delete manager;
}

void OtxFree::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case INDICATOR_DOMAIN_GENERAL:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/general");
            break;
        case INDICATOR_DOMAIN_GEO:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/geo");
            break;
        case INDICATOR_DOMAIN_HTTPSCANS:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/http_scans");
            break;
        case INDICATOR_DOMAIN_MALWARE:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/malware");
            break;
        case INDICATOR_DOMAIN_PASSIVEDNS:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/passive_dns");
            break;
        case INDICATOR_DOMAIN_URLLIST:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/url_list");
            break;
        case INDICATOR_DOMAIN_WHOIS:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/whois");
            break;
        case INDICATOR_HOSTNAME_GENERAL:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/hostname/"+args->target+"/general");
            break;
        case INDICATOR_HOSTNAME_GEO:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/hostname/"+args->target+"/geo");
            break;
        case INDICATOR_HOSTNAME_HTTPSCANS:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/hostname/"+args->target+"/http_scans");
            break;
        case INDICATOR_HOSTNAME_MALWARE:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/hostname/"+args->target+"/malware");
            break;
        case INDICATOR_HOSTNAME_PASSIVEDNS:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/hostname/"+args->target+"/passive_dns");
            break;
        case INDICATOR_HOSTNAME_URLLIST:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/hostname/"+args->target+"/url_list");
            break;
        case INDICATOR_IPV4_GENERAL:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv4/"+args->target+"/general");
            break;
        case INDICATOR_IPV4_GEO:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv4/"+args->target+"/geo");
            break;
        case INDICATOR_IPV4_MALWARE:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv4/"+args->target+"/malware");
            break;
        case INDICATOR_IPV4_PASSIVEDNS:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv4/"+args->target+"/passive_dns");
            break;
        case INDICATOR_IPV4_REPUTATION:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv4/"+args->target+"/reputation");
            break;
        case INDICATOR_IPV4_URLLIST:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv4/"+args->target+"/url_list");
            break;
        case INDICATOR_IPV6_GENERAL:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv6/"+args->target+"/general");
            break;
        case INDICATOR_IPV6_GEO:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv6/"+args->target+"/geo");
            break;
        case INDICATOR_IPV6_MALWARE:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv6/"+args->target+"/malware");
            break;
        case INDICATOR_IPV6_PASSIVEDNS:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv6/"+args->target+"/passive_dns");
            break;
        case INDICATOR_IPV6_REPUTATION:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv6/"+args->target+"/reputation");
            break;
        case INDICATOR_IPV6_URLLIST:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv6/"+args->target+"/url_list");
            break;
        case INDICATOR_URL_GENERAL:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/url/"+args->target+"/general");
            break;
        case INDICATOR_URL_URLLIST:
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/url/"+args->target+"/url_list");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputIp){
        /* if target ip-address contains  ":" then its an ipv6 */
        if(args->target.contains(":")){
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv4/"+args->target+"/passive_dns");
            request.setAttribute(QNetworkRequest::User, INDICATOR_IPV4_PASSIVEDNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        /* if target ip-address doesnt contains  ":" then its an ipv4 */
        else{
            url.setUrl("https://otx.alienvault.com/api/v1/indicators/IPv6/"+args->target+"/passive_dns");
            request.setAttribute(QNetworkRequest::User, INDICATOR_IPV6_PASSIVEDNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://otx.alienvault.com/api/v1/indicators/hostname/"+args->target+"/passive_dns");
        request.setAttribute(QNetworkRequest::User, INDICATOR_HOSTNAME_PASSIVEDNS);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void OtxFree::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray passive_dns = document.object()["passive_dns"].toArray();

    foreach(const QJsonValue &value, passive_dns){
        QString record_type = value.toObject()["record_type"].toString();
        if(record_type == "A" || record_type == "AAAA"){
            QString hostname = value.toObject()["hostname"].toString();
            QString address = value.toObject()["address"].toString();
            if(!address.startsWith("N")){ // avoid NXDOMAIN addresses...
                emit subdomainIp(hostname, address);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void OtxFree::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray passive_dns = document.object()["passive_dns"].toArray();

    foreach(const QJsonValue &value, passive_dns){
        QString record_type = value.toObject()["record_type"].toString();
        if(record_type == "A" || record_type == "AAAA"){
            QString hostname = value.toObject()["hostname"].toString();
            emit subdomain(hostname);
            log.resultsCount++;
        }
        if(record_type == "CNAME"){
            QString hostname = value.toObject()["address"].toString();
            emit CNAME(hostname);
            log.resultsCount++;
        }
        if(record_type == "NS"){
            QString hostname = value.toObject()["address"].toString();
            if(hostname != "NXDOMAIN"){
                emit NS(hostname);
                log.resultsCount++;
            }
        }
        if(record_type == "MX"){
            QString hostname = value.toObject()["address"].toString();
            emit MX(hostname);
            log.resultsCount++;
        }
    }
    end(reply);
}

void OtxFree::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray passive_dns = document.object()["passive_dns"].toArray();

    foreach(const QJsonValue &value, passive_dns){
        QString record_type = value.toObject()["record_type"].toString();
        if(record_type == "A"){
            QString address = value.toObject()["address"].toString();
            if(!address.startsWith("N")){ // avoid NXDOMAIN addresses...
                emit ipA(address);
                log.resultsCount++;
            }
        }
        if(record_type == "AAAA"){
            QString address = value.toObject()["address"].toString();
            if(!address.startsWith("N")){ // avoid NXDOMAIN addresses...
                emit ipAAAA(address);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void OtxFree::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray passive_dns = document.object()["passive_dns"].toArray();

    foreach(const QJsonValue &value, passive_dns){
        /* if asn value is null jump to next value */
        if(value.toObject()["asn"].isNull())
            continue;

        QString ASN = value.toObject()["asn"].toString();
        QStringList ASN_VALUE_NAME = ASN.split(" ");
        if(ASN_VALUE_NAME.size() == 2){
            emit asn(ASN_VALUE_NAME[0], ASN_VALUE_NAME[1]);
            log.resultsCount++;
        }
        if(ASN_VALUE_NAME.size() == 1){
            emit asn(ASN_VALUE_NAME[0], "");
            log.resultsCount++;
        }
    }
    end(reply);
}
