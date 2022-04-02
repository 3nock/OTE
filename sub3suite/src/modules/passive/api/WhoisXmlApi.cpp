#include "WhoisXmlApi.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define EMAIL_VERIFICATION 0
#define DNS_LOOKUP 1
#define DOMAIN_AVAILABILITY 2
#define IP_NETBLOCKS_ASN 3
#define IP_NETBLOCKS_CIDR 4
#define IP_NETBLOCKS_IP 5
#define IP_NETBLOCKS_ORG 6
#define IP_WHOIS 7
#define REVERSE_IP 8
#define REVERSE_MX 9
#define REVERSE_NS 10
#define REVERSE_WHOIS 11
#define SUBDOMAIN_LOOKUP 12
#define WEBSITE_CONTACTS 13
#define WEBSITE_SCRRENSHOT 14
#define WHOIS 15
#define WHOIS_HISTORY 16

/* 500 free queries a month
 * website contacts -> phone & email
 */
WhoisXmlApi::WhoisXmlApi(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_WHOISXMLAPI;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedSubdomain);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedIp);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedAsn);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedCidr);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedEmail);
    if(args.output_EnumNS)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedEnumNS);
    if(args.output_EnumMX)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedEnumMX);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_WHOISXMLAPI).toString();
}
WhoisXmlApi::~WhoisXmlApi(){
    delete manager;
}

void WhoisXmlApi::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id){
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
        case IP_NETBLOCKS_CIDR:
        {
            QString ip = target.split("/").at(0);
            QString mask = target.split("/").at(1);
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&ip="+ip+"&mask="+mask);
        }
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
        return;
    }

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            request.setAttribute(QNetworkRequest::User, SUBDOMAIN_LOOKUP);
            request.setUrl(url);
            manager->get(request);
            return;
        }

        if(args.output_Email){
            url.setUrl("https://website-contacts.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            request.setAttribute(QNetworkRequest::User, WEBSITE_CONTACTS);
            request.setUrl(url);
            manager->get(request);
            return;

            /*
            url.setUrl("https://whois-history.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target);
            request.setAttribute(QNetworkRequest::User, WHOIS_HISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            */
        }
    }

    if(args.input_IP){
        if(args.output_Hostname){
            url.setUrl("https://reverse-ip.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ip="+target);
            request.setAttribute(QNetworkRequest::User, REVERSE_IP);
            request.setUrl(url);
            manager->get(request);
            return;
        }

        if(args.output_ASN || args.output_CIDR || args.output_Email){
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&ip="+target);
            request.setAttribute(QNetworkRequest::User, IP_NETBLOCKS_IP);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_ASN){
        if(args.output_ASN || args.output_CIDR || args.output_Email){
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&asn="+target);
            request.setAttribute(QNetworkRequest::User, IP_NETBLOCKS_ASN);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_CIDR){
        if(args.output_ASN || args.output_CIDR || args.output_Email){
            QString ip = target.split("/").at(0);
            QString mask = target.split("/").at(1);
            url.setUrl("https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey="+m_key+"&ip="+ip+"&mask="+mask);
            request.setAttribute(QNetworkRequest::User, IP_NETBLOCKS_ASN);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    ///
    /// info...
    ///

    if(args.output_EnumMX){
        url.setUrl("https://reverse-mx.whoisxmlapi.com/api/v1?apiKey="+m_key+"&mx="+target);
        request.setAttribute(QNetworkRequest::User, REVERSE_MX);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumNS){
        url.setUrl("https://reverse-ns.whoisxmlapi.com/api/v1?apiKey="+m_key+"&ns="+target);
        request.setAttribute(QNetworkRequest::User, REVERSE_NS);
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

void WhoisXmlApi::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case SUBDOMAIN_LOOKUP:
    {
        QJsonArray records = result["records"].toArray();
        foreach(const QJsonValue &value, records){
            emit resultSubdomain(value.toObject()["domain"].toString());
            log.resultsCount++;
        }
    }
        break;

    case REVERSE_IP:
    {
        foreach(const QJsonValue &value, document.array()){
            QString hostname = value.toObject()["name"].toString();
            emit resultSubdomain(hostname);
            log.resultsCount++;
        }
    }
        break;

    case DNS_LOOKUP:
    {
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
    }

    this->end(reply);
}

void WhoisXmlApi::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_NETBLOCKS_IP:
    case IP_NETBLOCKS_ASN:
    case IP_NETBLOCKS_CIDR:
    {
        QJsonArray inetnums = document.object()["inetnums"].toArray();
        foreach(const QJsonValue &value, inetnums){
            QString asnValue = QString::number(value.toObject()["as"].toObject()["asn"].toInt());
            QString asnName  = value.toObject()["as"].toObject()["name"].toString();
            emit resultASN(asnValue, asnName);
            log.resultsCount++;
        }
    }
    }

    this->end(reply);
}

void WhoisXmlApi::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_NETBLOCKS_IP:
    case IP_NETBLOCKS_ASN:
    case IP_NETBLOCKS_CIDR:
    {
        QJsonArray inetnums = document.object()["inetnums"].toArray();
        foreach(const QJsonValue &value, inetnums){
            QString cidr = value.toObject()["as"].toObject()["route"].toString();
            emit resultCIDR(cidr);
            log.resultsCount++;
        }
    }
    }

    this->end(reply);
}

void WhoisXmlApi::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case WEBSITE_CONTACTS:
    {
        QJsonArray emails = document.object()["emails"].toArray();
        foreach(const QJsonValue &value, emails){
            emit resultEmail(value.toObject()["email"].toString());
            log.resultsCount++;
        }
    }
        break;

    case IP_NETBLOCKS_IP:
    case IP_NETBLOCKS_ASN:
    case IP_NETBLOCKS_CIDR:
    {
        QJsonArray inetnums = document.object()["inetnums"].toArray();
        foreach(const QJsonValue &value, inetnums){
            foreach(const QJsonValue &adminContact, value.toObject()["adminContact"].toArray()){
                emit resultEmail(adminContact.toObject()["email"].toString());
                log.resultsCount++;
            }
            foreach(const QJsonValue &adminContact, value.toObject()["techContact"].toArray()){
                emit resultEmail(adminContact.toObject()["email"].toString());
                log.resultsCount++;
            }
            foreach(const QJsonValue &adminContact, value.toObject()["abuseContact"].toArray()){
                emit resultEmail(adminContact.toObject()["email"].toString());
                log.resultsCount++;
            }
        }
    }
        break;

    /*
    case WHOIS_HISTORY:
    {
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
    */
    }

    this->end(reply);
}

///
/// Info...
///

void WhoisXmlApi::replyFinishedEnumNS(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray result = document.object()["result"].toArray();

    s3s_struct::NS ns;
    ns.ns = target;

    foreach(const QJsonValue &domain, result)
        ns.domains.insert(domain.toObject()["name"].toString());

    if(!ns.domains.isEmpty())
        emit resultEnumNS(ns);

    this->end(reply);
}

void WhoisXmlApi::replyFinishedEnumMX(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray result = document.object()["result"].toArray();

    s3s_struct::MX mx;
    mx.mx = target;

    foreach(const QJsonValue &domain, result)
        mx.domains.insert(domain.toObject()["name"].toString());

    if(!mx.domains.isEmpty())
        emit resultEnumMX(mx);

    this->end(reply);
}
