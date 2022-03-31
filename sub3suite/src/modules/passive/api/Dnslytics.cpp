#include "Dnslytics.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define ACCOUNTINFO 0
#define ASINFO 1
#define DOMAINSEARCH 2
#define DOMAINTYPOS 3
#define HOSTINGHISTORY 4
#define IP2ASN 5
#define IPINFO 6
#define REVERSEADSENSE 7
#define REVERSEGANALYTICS 8
#define REVERSEIP 9
#define REVERSEMX 10
#define REVERSENS 11
#define SUBNETINFO 12


Dnslytics::Dnslytics(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_DNSLYTICS;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedSubdomain);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedIp);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedAsn);
    if(args.output_HostnameIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedSubdomainIp);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedCidr);
    if(args.output_EnumIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedEnumIP);
    if(args.output_EnumMX)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedEnumMX);
    if(args.output_EnumNS)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedEnumNS);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_DNSLYTICS).toString();
}
Dnslytics::~Dnslytics(){
    delete manager;
}

void Dnslytics::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case ACCOUNTINFO:
            url.setUrl("https://api.dnslytics.net/v1/accountinfo?apikey="+m_key);
            break;
        case ASINFO:
            url.setUrl("https://api.dnslytics.net/v1/asinfo/"+target+"/summary?apikey="+m_key);
            break;
        case DOMAINSEARCH:
            url.setUrl("https://api.dnslytics.net/v1/domainsearch/"+target+"?apikey="+m_key);
            break;
        case DOMAINTYPOS:
            url.setUrl("https://api.dnslytics.net/v1/domaintypos/"+target+"?apikey="+m_key);
            break;
        case HOSTINGHISTORY:
            url.setUrl("https://api.dnslytics.net/v1/hostinghistory/"+target+"?apikey="+m_key);
            break;
        case IPINFO:
            url.setUrl("https://api.dnslytics.net/v1/ipinfo/"+target+"?apikey="+m_key);
            break;
        case IP2ASN:
            url.setUrl("https://freeapi.dnslytics.net/v1/ip2asn/"+target);
            break;
        case REVERSEADSENSE:
            url.setUrl("https://api.dnslytics.net/v1/reverseadsense/"+target+"?apikey="+m_key);
            break;
        case REVERSEGANALYTICS:
            url.setUrl("https://api.dnslytics.net/v1/reverseganalytics/"+target+"?apikey="+m_key);
            break;
        case REVERSEIP:
            url.setUrl("https://api.dnslytics.net/v1/reverseip/"+target+"?apikey="+m_key);
            break;
        case REVERSEMX:
            url.setUrl("https://api.dnslytics.net/v1/reversemx/"+target+"?apikey="+m_key);
            break;
        case REVERSENS:
            url.setUrl("https://api.dnslytics.net/v1/reversens/"+target+"?apikey="+m_key);
            break;
        case SUBNETINFO:
            url.setUrl("https://api.dnslytics.net/v1/subnetinfo/"+target+"/summary?apikey="+m_key);
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    ///
    /// for Enums...
    ///

    if(args.output_EnumIP){
        url.setUrl("https://api.dnslytics.net/v1/reverseip/"+target+"?apikey="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumNS){
        url.setUrl("https://api.dnslytics.net/v1/reversens/"+target+"?apikey="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumMX){
        url.setUrl("https://api.dnslytics.net/v1/reversemx/"+target+"?apikey="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    ///
    /// for osint output...
    ///

    if(args.input_Search){
        if(args.output_Hostname){
            url.setUrl("https://api.dnslytics.net/v1/domainsearch/"+target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAINSEARCH);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://api.dnslytics.net/v1/domainsearch/"+target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAINSEARCH);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_IP){
            url.setUrl("https://api.dnslytics.net/v1/hostinghistory/"+target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, HOSTINGHISTORY);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_IP){
        if(args.output_Hostname){
            url.setUrl("https://api.dnslytics.net/v1/reverseip/"+target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, REVERSEIP);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_ASN || args.output_CIDR){
            url.setUrl("https://freeapi.dnslytics.net/v1/ip2asn/"+target);
            request.setAttribute(QNetworkRequest::User, IP2ASN);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_ASN){
        if(args.output_IP || args.output_ASN || args.output_Hostname || args.output_HostnameIP || args.output_CIDR){
            url.setUrl("https://api.dnslytics.net/v1/asinfo/"+target+"/summary?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, ASINFO);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_CIDR){
        if(args.output_IP || args.output_ASN || args.output_Hostname || args.output_HostnameIP || args.output_CIDR){
            url.setUrl("https://api.dnslytics.net/v1/subnetinfo/"+target+"/summary?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, SUBNETINFO);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void Dnslytics::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASINFO:
    case SUBNETINFO:
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString mxrecord = value.toObject()["mxrecord"].toString();
            emit resultMX(mxrecord);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString nsrecord = value.toObject()["nsrecord"].toString();
            emit resultNS(nsrecord);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            emit resultSubdomain(domain);
            log.resultsCount++;
        }
        break;

    case DOMAINSEARCH:
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            emit resultSubdomain(domain);
            log.resultsCount++;
        }
        break;

    case REVERSEIP:
        foreach(const QJsonValue &value, data["domains"].toArray()){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Dnslytics::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASINFO:
    case SUBNETINFO:
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString mxrecord = value.toObject()["mxrecord"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultSubdomainIp(mxrecord, ipv4);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString nsrecord = value.toObject()["nsrecord"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultSubdomainIp(nsrecord, ipv4);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultSubdomainIp(domain, ipv4);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Dnslytics::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASINFO:
        foreach(const QJsonValue &value, data["peersv4"].toArray()){
            QString asn = QString::number(value.toObject()["aspeer"].toInt());
            QString name = value.toObject()["shortname"].toString();
            emit resultASN(asn, name);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["peersv6"].toArray()){
            QString asn = QString::number(value.toObject()["aspeer"].toInt());
            QString name = value.toObject()["shortname"].toString();
            emit resultASN(asn, name);
            log.resultsCount++;
        }
        break;

    case SUBNETINFO:
        foreach(const QJsonValue &value, data["routes"].toArray()){
            QString asn = QString::number(value.toObject()["asn"].toInt());
            QString name = value.toObject()["shortname"].toString();
            emit resultASN(asn, name);
            log.resultsCount++;
        }
        break;

    case IP2ASN:
        QJsonObject mainObj = document.object();
        QString asn = QString::number(mainObj["asn"].toInt());
        QString name = mainObj["shortname"].toString();

        emit resultASN(asn, name);
        log.resultsCount++;
    }

    this->end(reply);
}

void Dnslytics::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASINFO:
    case SUBNETINFO:
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultA(ipv4);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultA(ipv4);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultA(ipv4);
            log.resultsCount++;
        }
        break;

    case HOSTINGHISTORY:
        foreach(const QJsonValue &value, data["ipv6"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultAAAA(ip);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["ipv4"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultA(ip);
            log.resultsCount++;
        }
        break;
    }

    this->end(reply);
}

void Dnslytics::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASINFO:
        foreach(const QJsonValue &value, data["prefixesv4"].toArray()){
            QString Prefix = value.toObject()["prefix"].toString();
            emit resultCIDR(Prefix);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["prefixesv6"].toArray()){
            QString Prefix = value.toObject()["prefix"].toString();
            emit resultCIDR(Prefix);
            log.resultsCount++;
        }
        break;

    case SUBNETINFO:
        foreach(const QJsonValue &value, data["routes"].toArray()){
            QString cidr = value.toObject()["cidr"].toString();
            emit resultCIDR(cidr);
            log.resultsCount++;
        }
        break;

    case IP2ASN:
        QJsonObject mainObj = document.object();
        QString cidr = mainObj["cidr"].toString();

        emit resultCIDR(cidr);
        log.resultsCount++;
        break;
    }

    this->end(reply);
}

///
/// For info...
///

void Dnslytics::replyFinishedEnumMX(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    s3s_struct::MX mx;
    mx.mx = target;

    foreach(const QJsonValue &value, data["domains"].toArray())
        mx.domains.insert(value.toString());

    emit resultEnumMX(mx);

    this->end(reply);
}

void Dnslytics::replyFinishedEnumNS(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    s3s_struct::NS ns;
    ns.ns = target;

    foreach(const QJsonValue &value, data["domains"].toArray())
        ns.domains.insert(value.toString());

    emit resultEnumNS(ns);

    this->end(reply);
}

void Dnslytics::replyFinishedEnumIP(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    s3s_struct::IP ip;
    ip.ip = target;

    foreach(const QJsonValue &value, data["domains"].toArray())
        ip.domains.insert(value.toString());

    emit resultEnumIP(ip);

    this->end(reply);
}
