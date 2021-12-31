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
WhoisXmlApi::WhoisXmlApi(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "WhoisXmlApi";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedAsn);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedEmail);
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
    if(args.outputRaw){
        switch (args.rawOption){
        case WHOIS:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&domainName="+target);
            break;
        case IP_WHOIS:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&ipWhois=1&ip=1&domainName="+target);
            break;
        case DNS_LOOKUP:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/DNSService?apiKey="+m_key+"&domainName="+target+"&type=_subdomainIp");
            break;
        case EMAIL_VERIFICATION:
            url.setUrl("https://emailverification.whoisxmlapi.com/api/v1?apiKey="+m_key+"&emailAddress="+target);
            break;
        case SUBDOMAIN_LOOKUP:
            url.setUrl("https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            break;
        case WEBSITE_SCRRENSHOT:
            url.setUrl("https://website-screenshot.whoisxmlapi.com/api/v1?apiKey="+m_key+"&url="+target);
            break;
        case WHOIS_HISTORY:
            url.setUrl("https://whois-history.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            break;
        case REVERSE_WHOIS:
            url.setUrl("https://reverse-whois.whoisxmlapi.com/api/v2");
            /* a post request */
            break;
        case REVERSE_NS:
            url.setUrl("https://reverse-ns.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ns="+target);
            break;
        case REVERSE_MX:
            url.setUrl("https://reverse-mx.whoisxmlapi.com/api/v1?apiKey="+m_key+"&mx="+target);
            break;
        case REVERSE_IP:
            url.setUrl("https://reverse-ip.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ip="+target);
            break;
        case IP_NETBLOCKS_IP:
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&ip="+target);
            break;
        case IP_NETBLOCKS_ASN:
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&asn="+target);
            break;
        case IP_NETBLOCKS_ORG:
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&org[]="+target);
            break;
        case DOMAIN_AVAILABILITY:
            url.setUrl("https://domain-availability.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            break;
        case WEBSITE_CONTACTS:
            url.setUrl("https://website-contacts.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomain){
            url.setUrl("https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            request.setAttribute(QNetworkRequest::User, SUBDOMAIN_LOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputEmail){
            url.setUrl("https://whois-history.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            request.setAttribute(QNetworkRequest::User, WHOIS_HISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://website-contacts.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            request.setAttribute(QNetworkRequest::User, WEBSITE_CONTACTS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputIp){
        if(args.outputSubdomain){
            url.setUrl("https://reverse-ip.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ip="+target);
            request.setAttribute(QNetworkRequest::User, REVERSE_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputAsn){
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&ip="+target);
            request.setAttribute(QNetworkRequest::User, IP_NETBLOCKS_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputAsn){
        if(args.outputAsn){
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&asn="+target);
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(QUERY_TYPE == SUBDOMAIN_LOOKUP){
        QJsonArray records = result["records"].toArray();
        foreach(const QJsonValue &value, records){
            emit resultSubdomain(value.toObject()["domain"].toString());
            log.resultsCount++;
        }
    }

    if(QUERY_TYPE == REVERSE_IP){
        foreach(const QJsonValue &value, document.array()){
            QString hostname = value.toObject()["name"].toString();
            emit resultSubdomain(hostname);
            log.resultsCount++;
        }
    }

    if(QUERY_TYPE == DNS_LOOKUP){
        QJsonArray dnsRecords = document.object()["dnsRecords"].toArray();
        foreach(const QJsonValue &record, dnsRecords){
            QString dnsType = record.toObject()["dnsType"].toString();

            if(dnsType == "MX"){
                QString hostname = record.toObject()["target"].toString();
                emit resultMX(hostname);
                log.resultsCount++;
            }
            if(dnsType == "NS"){
                QString hostname = record.toObject()["target"].toString();
                emit resultNS(hostname);
                log.resultsCount++;
            }
            if(dnsType == "CNAME"){
                QString hostname = record.toObject()["target"].toString();
                emit resultCNAME(hostname);
                log.resultsCount++;
            }
            if(dnsType == "TXT"){
                QJsonArray strings = record.toObject()["strings"].toArray();
                foreach(const QJsonValue &value, strings){
                    emit resultTXT(value.toString());
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(QUERY_TYPE == IP_NETBLOCKS_IP || QUERY_TYPE == IP_NETBLOCKS_ASN){
        QJsonArray inetnums = document.object()["inetnums"].toArray();
        foreach(const QJsonValue &value, inetnums){
            QString asnValue = QString::number(value.toObject()["as"].toObject()["asn"].toInt());
            QString asnName  = value.toObject()["as"].toObject()["name"].toString();
            emit resultASN(asnValue, asnName);
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(QUERY_TYPE == WHOIS_HISTORY){
        QJsonArray records = result["records"].toArray();
        foreach(const QJsonValue &value, records){
            QString registrantContact = value.toObject()["registrantContact"].toObject()["email"].toString();
            QString administrativeContact = value.toObject()["administrativeContact"].toObject()["email"].toString();
            QString technicalContact = value.toObject()["technicalContact"].toObject()["email"].toString();
            QString billingContact = value.toObject()["billingContact"].toObject()["email"].toString();
            QString zoneContact = value.toObject()["zoneContact"].toObject()["email"].toString();

            if(!registrantContact.isEmpty()){
                emit resultEmail(registrantContact);
                log.resultsCount++;
            }
            if(!administrativeContact.isEmpty()){
                emit resultEmail(administrativeContact);
                log.resultsCount++;
            }
            if(!technicalContact.isEmpty()){
                emit resultEmail(technicalContact);
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == WEBSITE_CONTACTS){
        QJsonArray emails = document.object()["emails"].toArray();
        foreach(const QJsonValue &value, emails){
            emit resultEmail(value.toObject()["email"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
