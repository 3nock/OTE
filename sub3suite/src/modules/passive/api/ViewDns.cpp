#include "ViewDns.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ABUSE_CONTACT_LOOKUP 0
#define DNS_PROPAGATION_CHECKER 1
#define DNS_RECORD_LOOKUP 2
#define DOMAIN_IP_WHOIS 3
#define IP_HISTORY 4
#define IP_LOCATION_FINDER 5
#define MAC_ADDRESS_LOOKUP 6
#define PING 7
#define PORT_SCANNER 8
#define REVERSE_DNS_LOOKUP 9
#define REVERSE_IP_LOOKUP 10
#define REVERSE_MX_LOOKUP 11
#define REVERSE_NS_LOOKUP 12
#define REVERSE_WHOIS_LOOKUP 13

/*
 * only 250 api calls available for free...
 */
ViewDns::ViewDns(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_VIEWDNS;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ViewDns::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ViewDns::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ViewDns::replyFinishedIp);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ViewDns::replyFinishedEmail);
    if(args.outputInfoNS)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ViewDns::replyFinishedInfoNS);
    if(args.outputInfoMX)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ViewDns::replyFinishedInfoMX);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_VIEWDNS).toString();
}
ViewDns::~ViewDns(){
    delete manager;
}

void ViewDns::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ABUSE_CONTACT_LOOKUP:
            url.setUrl("https://api.viewdns.info/abuselookup/?domain="+target+"&apikey="+m_key+"&output=json");
            break;
        case DNS_PROPAGATION_CHECKER:
            url.setUrl("https://api.viewdns.info/propagation/?domain="+target+"&apikey="+m_key+"&output=json");
            break;
        case DNS_RECORD_LOOKUP:
            url.setUrl("https://api.viewdns.info/dnsrecord/?domain="+target+"&recordtype=ANY&apikey="+m_key+"&output=json");
            break;
        case DOMAIN_IP_WHOIS:
            url.setUrl("https://api.viewdns.info/whois/?domain="+target+"&apikey="+m_key+"&output=json");
            break;
        case IP_HISTORY:
            url.setUrl("https://api.viewdns.info/iphistory/?domain="+target+"&apikey="+m_key+"&output=json");
            break;
        case IP_LOCATION_FINDER:
            url.setUrl("https://api.viewdns.info/iplocation/?ip="+target+"&apikey="+m_key+"&output=json");
            break;
        case MAC_ADDRESS_LOOKUP:
            url.setUrl("https://api.viewdns.info/maclookup/?mac="+target+"&apikey="+m_key+"&output=json");
            break;
        case PING:
            url.setUrl("https://api.viewdns.info/ping/?host="+target+"&apikey="+m_key+"&output=json");
            break;
        case PORT_SCANNER:
            url.setUrl("https://api.viewdns.info/portscan/?host="+target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_DNS_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversedns/?ip="+target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_IP_LOOKUP:
            url.setUrl("https://api.viewdns.info/reverseip/?host="+target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_MX_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversemx/?mx="+target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_NS_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversens/?ns="+target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_WHOIS_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversewhois/?q="+target+"&apikey="+m_key+"&output=json");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomain || args.outputIp){
            url.setUrl("https://api.viewdns.info/dnsrecord/?domain="+target+"&recordtype=ANY&apikey="+m_key+"&output=json");
            request.setAttribute(QNetworkRequest::User, DNS_RECORD_LOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputIp){
            url.setUrl("https://api.viewdns.info/iphistory/?domain="+target+"&apikey="+m_key+"&output=json");
            request.setAttribute(QNetworkRequest::User, IP_HISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputEmail){
            url.setUrl("https://api.viewdns.info/abuselookup/?domain="+target+"&apikey="+m_key+"&output=json");
            request.setAttribute(QNetworkRequest::User, ABUSE_CONTACT_LOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputIp){
        if(args.outputSubdomain){
            url.setUrl("https://api.viewdns.info/reverseip/?host="+target+"&apikey="+m_key+"&output=json");
            request.setAttribute(QNetworkRequest::User, REVERSE_IP_LOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputEmail){
        if(args.outputSubdomain){
            url.setUrl("https://api.viewdns.info/reversewhois/?q="+target+"&apikey="+m_key+"&output=json");
            request.setAttribute(QNetworkRequest::User, REVERSE_WHOIS_LOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    ///
    /// info...
    ///

    if(args.outputInfoMX){
        url.setUrl("https://api.viewdns.info/reversemx/?mx="+target+"&apikey="+m_key+"&output=json");
        request.setAttribute(QNetworkRequest::User, REVERSE_MX_LOOKUP);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args.outputInfoNS){
        url.setUrl("https://api.viewdns.info/reversens/?ns="+target+"&apikey="+m_key+"&output=json");
        request.setAttribute(QNetworkRequest::User, REVERSE_NS_LOOKUP);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void ViewDns::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DNS_RECORD_LOOKUP:
    {
        QJsonArray records = response["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString type = record.toObject()["type"].toString();

            if(type == "NS"){
                QString data = record.toObject()["data"].toString();
                emit resultNS(data);
                log.resultsCount++;
            }
            if(type == "MX"){
                QString data = record.toObject()["data"].toString();
                emit resultMX(data);
                log.resultsCount++;
            }
            if(type == "CNAME"){
                QString data = record.toObject()["data"].toString();
                emit resultCNAME(data);
                log.resultsCount++;
            }
            if(type == "TXT"){
                QString data = record.toObject()["data"].toString();
                emit resultTXT(data);
                log.resultsCount++;
            }
        }
    }
        break;

    case REVERSE_IP_LOOKUP:
    {
        QJsonArray domains = response["domains"].toArray();
        foreach(const QJsonValue &domain, domains){
            QString hostname = domain.toObject()["name"].toString();
            emit resultSubdomain(hostname);
            log.resultsCount++;
        }
    }
        break;

    case REVERSE_WHOIS_LOOKUP:
    {
        QJsonArray matches = response["matches"].toArray();
        foreach(const QJsonValue &match, matches){
            QString hostname = match.toObject()["domain"].toString();
            emit resultSubdomain(hostname);
            log.resultsCount++;
        }
    }
    }

    end(reply);
}

void ViewDns::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ABUSE_CONTACT_LOOKUP:
        emit resultEmail(response["abusecontact"].toString());
        log.resultsCount++;
    }

    end(reply);
}

void ViewDns::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();
    QJsonArray records = response["records"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DNS_RECORD_LOOKUP:
        foreach(const QJsonValue &record, records){
            QString type = record.toObject()["type"].toString();

            if(type == "A"){
                QString data = record.toObject()["data"].toString();
                emit resultA(data);
                log.resultsCount++;
            }
            if(type == "AAAA"){
                QString data = record.toObject()["data"].toString();
                emit resultAAAA(data);
                log.resultsCount++;
            }
        }
        break;

    case IP_HISTORY:
        foreach(const QJsonValue &record, records){
            QString address = record.toObject()["ip"].toString();
            emit resultIP(address);
            log.resultsCount++;
        }
    }

    end(reply);
}

///
/// Info...
///

void ViewDns::replyFinishedInfoNS(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();
    QJsonArray domains = response["domains"].toArray();

    s3s_struct::NS ns;
    ns.info_ns = target;

    foreach(const QJsonValue &domain, domains)
        ns.domains.insert(domain.toObject()["domain"].toString());

    emit infoNS(ns);

    end(reply);
}

void ViewDns::replyFinishedInfoMX(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();
    QJsonArray domains = response["domains"].toArray();

    s3s_struct::MX mx;
    mx.info_mx = target;

    foreach(const QJsonValue &domain, domains)
        mx.domains.insert(domain.toString());

    emit infoMX(mx);

    end(reply);
}
