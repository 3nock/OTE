#include "WhoisXmlApi.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define EMAIL_VERIFICATION 0
#define DNS_LOOKUP 1
#define DOMAIN_AVAILABILITY 2
#define IP_NETBLOCKS_ASN 3
#define IP_NETBLOCKS_IP 4
#define IP_NETBLOCKS_ORG 5
#define IP_WHOIS 6
#define REVERSE_IP 7
#define REVERSE_MX 8
#define REVERSE_NS 9
#define REVERSE_WHOIS 10
#define SUBDOMAIN_LOOKUP 11
#define WEBSITE_CONTACTS 12
#define WEBSITE_SCRRENSHOT 13
#define WHOIS 14
#define WHOIS_HISTORY 15

/* 500 free queries a month */
/* has reverse ns,mx and ip */
/* also returns ip-netblocks */
/* domain availability if domain can be purchased */
/* good whois for domain & ip*/
/* website contacts */
WhoisXmlApi::WhoisXmlApi(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "WhoisXmlApi";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedAsn);
    if(args->outputEmail)
        connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedEmail);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("whoisxmlapi").toString();
    Config::generalConfig().endGroup();
}
WhoisXmlApi::~WhoisXmlApi(){
    delete manager;
}

void WhoisXmlApi::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args->raw){
        switch (args->rawOption){
        case WHOIS:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&domainName="+args->target);
            break;
        case IP_WHOIS:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&ipWhois=1&ip=1&domainName="+args->target);
            break;
        case DNS_LOOKUP:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/DNSService?apiKey="+m_key+"&domainName="+args->target+"&type=_subdomainIp");
            break;
        case EMAIL_VERIFICATION:
            url.setUrl("https://emailverification.whoisxmlapi.com/api/v1?apiKey="+m_key+"&emailAddress="+args->target);
            break;
        case SUBDOMAIN_LOOKUP:
            url.setUrl("https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
            break;
        case WEBSITE_SCRRENSHOT:
            url.setUrl("https://website-screenshot.whoisxmlapi.com/api/v1?apiKey="+m_key+"&url="+args->target);
            break;
        case WHOIS_HISTORY:
            url.setUrl("https://whois-history.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
            break;
        case REVERSE_WHOIS:
            url.setUrl("https://reverse-whois.whoisxmlapi.com/api/v2");
            /* a post request */
            break;
        case REVERSE_NS:
            url.setUrl("https://reverse-ns.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ns="+args->target);
            break;
        case REVERSE_MX:
            url.setUrl("https://reverse-mx.whoisxmlapi.com/api/v1?apiKey="+m_key+"&mx="+args->target);
            break;
        case REVERSE_IP:
            url.setUrl("https://reverse-ip.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ip="+args->target);
            break;
        case IP_NETBLOCKS_IP:
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&ip="+args->target);
            break;
        case IP_NETBLOCKS_ASN:
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&asn="+args->target);
            break;
        case IP_NETBLOCKS_ORG:
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&org[]="+args->target);
            break;
        case DOMAIN_AVAILABILITY:
            url.setUrl("https://domain-availability.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
            break;
        case WEBSITE_CONTACTS:
            url.setUrl("https://website-contacts.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
            request.setAttribute(QNetworkRequest::User, SUBDOMAIN_LOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputEmail){
            url.setUrl("https://whois-history.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
            request.setAttribute(QNetworkRequest::User, WHOIS_HISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://website-contacts.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
            request.setAttribute(QNetworkRequest::User, WEBSITE_CONTACTS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args->inputIp){
        if(args->outputSubdomain){
            url.setUrl("https://reverse-ip.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ip="+args->target);
            request.setAttribute(QNetworkRequest::User, REVERSE_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputAsn){
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&ip="+args->target);
            request.setAttribute(QNetworkRequest::User, IP_NETBLOCKS_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args->inputAsn){
        if(args->outputAsn){
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&asn="+args->target);
            request.setAttribute(QNetworkRequest::User, IP_NETBLOCKS_ASN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void WhoisXmlApi::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(requestType == SUBDOMAIN_LOOKUP){
        QJsonArray records = result["records"].toArray();
        foreach(const QJsonValue &value, records){
            emit subdomain(value.toObject()["domain"].toString());
            log.resultsCount++;
        }
    }

    if(requestType == REVERSE_IP){
        foreach(const QJsonValue &value, document.array()){
            QString hostname = value.toObject()["name"].toString();
            emit subdomain(hostname);
            log.resultsCount++;
        }
    }

    if(requestType == DNS_LOOKUP){
        QJsonArray dnsRecords = document.object()["dnsRecords"].toArray();
        foreach(const QJsonValue &record, dnsRecords){
            QString dnsType = record.toObject()["dnsType"].toString();

            if(dnsType == "MX"){
                QString hostname = record.toObject()["target"].toString();
                emit MX(hostname);
                log.resultsCount++;
            }
            if(dnsType == "NS"){
                QString hostname = record.toObject()["target"].toString();
                emit NS(hostname);
                log.resultsCount++;
            }
            if(dnsType == "CNAME"){
                QString hostname = record.toObject()["target"].toString();
                emit CNAME(hostname);
                log.resultsCount++;
            }
            if(dnsType == "TXT"){
                QJsonArray strings = record.toObject()["strings"].toArray();
                foreach(const QJsonValue &value, strings){
                    emit TXT(value.toString());
                }
            }
        }
    }

    end(reply);
}

void WhoisXmlApi::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == IP_NETBLOCKS_IP || requestType == IP_NETBLOCKS_ASN){
        QJsonArray inetnums = document.object()["inetnums"].toArray();
        foreach(const QJsonValue &value, inetnums){
            QString asnValue = QString::number(value.toObject()["as"].toObject()["asn"].toInt());
            QString asnName  = value.toObject()["as"].toObject()["name"].toString();
            emit asn(asnValue, asnName);
            log.resultsCount++;
        }
    }

    end(reply);
}

void WhoisXmlApi::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(requestType == WHOIS_HISTORY){
        QJsonArray records = result["records"].toArray();
        foreach(const QJsonValue &value, records){
            QString registrantContact = value.toObject()["registrantContact"].toObject()["email"].toString();
            QString administrativeContact = value.toObject()["administrativeContact"].toObject()["email"].toString();
            QString technicalContact = value.toObject()["technicalContact"].toObject()["email"].toString();
            QString billingContact = value.toObject()["billingContact"].toObject()["email"].toString();
            QString zoneContact = value.toObject()["zoneContact"].toObject()["email"].toString();

            if(!registrantContact.isEmpty()){
                emit email(registrantContact);
                log.resultsCount++;
            }
            if(!administrativeContact.isEmpty()){
                emit email(administrativeContact);
                log.resultsCount++;
            }
            if(!technicalContact.isEmpty()){
                emit email(technicalContact);
                log.resultsCount++;
            }
        }
    }

    if(requestType == WEBSITE_CONTACTS){
        QJsonArray emails = document.object()["emails"].toArray();
        foreach(const QJsonValue &value, emails){
            emit email(value.toObject()["email"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
