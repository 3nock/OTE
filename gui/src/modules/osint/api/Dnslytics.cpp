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

/*
 * has reverse ip, reverse mx and reverse ns
 */
Dnslytics::Dnslytics(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Dnslytics";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnslytics::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnslytics::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnslytics::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnslytics::replyFinishedAsn);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnslytics::replyFinishedSubdomainIp);
    if(args->outputPrefixes)
        connect(manager, &MyNetworkAccessManager::finished, this, &Dnslytics::replyFinishedPrefixes);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("dnslytics").toString();
    Config::generalConfig().endGroup();
}
Dnslytics::~Dnslytics(){
    delete manager;
}

void Dnslytics::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case ACCOUNTINFO:
            url.setUrl("https://api.dnslytics.net/v1/accountinfo?apikey="+m_key);
            break;
        case ASINFO:
            url.setUrl("https://api.dnslytics.net/v1/asinfo/"+args->target+"/summary?apikey="+m_key);
            break;
        case DOMAINSEARCH:
            url.setUrl("https://api.dnslytics.net/v1/domainsearch/"+args->target+"?apikey="+m_key);
            break;
        case DOMAINTYPOS:
            url.setUrl("https://api.dnslytics.net/v1/domaintypos/"+args->target+"?apikey="+m_key);
            break;
        case HOSTINGHISTORY:
            url.setUrl("https://api.dnslytics.net/v1/hostinghistory/"+args->target+"?apikey="+m_key);
            break;
        case IPINFO:
            url.setUrl("https://api.dnslytics.net/v1/ipinfo/"+args->target+"?apikey="+m_key);
            break;
        case IP2ASN:
            url.setUrl("https://freeapi.dnslytics.net/v1/ip2asn/"+args->target);
            break;
        case REVERSEADSENSE:
            url.setUrl("https://api.dnslytics.net/v1/reverseadsense/"+args->target+"?apikey="+m_key);
            break;
        case REVERSEGANALYTICS:
            url.setUrl("https://api.dnslytics.net/v1/reverseganalytics/"+args->target+"?apikey="+m_key);
            break;
        case REVERSEIP:
            url.setUrl("https://api.dnslytics.net/v1/reverseip/"+args->target+"?apikey="+m_key);
            break;
        case REVERSEMX:
            url.setUrl("https://api.dnslytics.net/v1/reversemx/"+args->target+"?apikey="+m_key);
            break;
        case REVERSENS:
            url.setUrl("https://api.dnslytics.net/v1/reversens/"+args->target+"?apikey="+m_key);
            break;
        case SUBNETINFO:
            url.setUrl("https://api.dnslytics.net/v1/subnetinfo/"+args->target+"/summary?apikey="+m_key);
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.dnslytics.net/v1/domainsearch/"+args->target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAINSEARCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }

        if(args->outputIp || args->outputSubdomain){
            url.setUrl("https://api.dnslytics.net/v1/hostinghistory/"+args->target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, HOSTINGHISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args->inputAsn){
        if(args->outputIp || args->outputAsn || args->outputSubdomain || args->outputSubdomainIp || args->outputPrefixes){
            url.setUrl("https://api.dnslytics.net/v1/asinfo/"+args->target+"/summary?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, ASINFO);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }

        if(args->outputSubdomain){
            url.setUrl("https://api.dnslytics.net/v1/reverseip/"+args->target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, REVERSEIP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }
}

void Dnslytics::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == ASINFO){
        /* mxrecords */
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString mxrecord = value.toObject()["mxrecord"].toString();
            emit MX(mxrecord);
            log.resultsCount++;
        }
        /* nsrecords */
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString nsrecord = value.toObject()["nsrecord"].toString();
            emit NS(nsrecord);
            log.resultsCount++;
        }
        /* domains */
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            emit subdomain(domain);
            log.resultsCount++;
        }
    }

    if(requestType == DOMAINSEARCH){
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            emit subdomain(domain);
            log.resultsCount++;
        }
    }

    if(requestType == HOSTINGHISTORY){
        /* mxrecords */
        foreach(const QJsonValue &value, data["mx"].toArray()){
            QString mx = value.toObject()["mx"].toString();
            emit MX(mx);
            log.resultsCount++;
        }
        /* nsrecords */
        foreach(const QJsonValue &value, data["ns"].toArray()){
            QString ns = value.toObject()["ns"].toString();
            emit NS(ns);
            log.resultsCount++;
        }
        /* domains */
        foreach(const QJsonValue &value, data["dns"].toArray()){
            QString dns = value.toObject()["dns"].toString();
            emit subdomain(dns);
            log.resultsCount++;
        }
    }

    if(requestType == REVERSEIP){
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toString();
            emit subdomain(domain);
            log.resultsCount++;
        }
    }

    end(reply);
}

void Dnslytics::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == ASINFO){
        /* mxrecords */
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString mxrecord = value.toObject()["mxrecord"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit subdomainIp(mxrecord, ipv4);
            log.resultsCount++;
        }
        /* nsrecords */
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString nsrecord = value.toObject()["nsrecord"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit subdomainIp(nsrecord, ipv4);
            log.resultsCount++;
        }
        /* domains */
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit subdomainIp(domain, ipv4);
            log.resultsCount++;
        }
    }


    end(reply);
}

void Dnslytics::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == ASINFO){
        /* peersv4 */
        foreach(const QJsonValue &value, data["peersv4"].toArray()){
            QString aspeer = value.toObject()["aspeer"].toString();
            QString shortname = value.toObject()["shortname"].toString();
            emit asn(aspeer, shortname);
            log.resultsCount++;
        }
        /* peersv6 */
        foreach(const QJsonValue &value, data["peersv6"].toArray()){
            QString aspeer = value.toObject()["aspeer"].toString();
            QString shortname = value.toObject()["shortname"].toString();
            emit asn(aspeer, shortname);
            log.resultsCount++;
        }
    }

    end(reply);
}

void Dnslytics::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == ASINFO){
        /* mxrecords */
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit ipA(ipv4);
            log.resultsCount++;
        }
        /* nsrecords */
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit ipA(ipv4);
            log.resultsCount++;
        }
        /* domains */
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit ipA(ipv4);
            log.resultsCount++;
        }
    }

    if(requestType == HOSTINGHISTORY){
        /* ipv6 */
        foreach(const QJsonValue &value, data["ipv6"].toArray()){
            QString ipv6 = value.toObject()["ip"].toString();
            emit ipAAAA(ipv6);
            log.resultsCount++;
        }
        /* ipv4 */
        foreach(const QJsonValue &value, data["ipv4"].toArray()){
            QString ipv4 = value.toObject()["ip"].toString();
            emit ipA(ipv4);
            log.resultsCount++;
        }
    }

    end(reply);
}

void Dnslytics::replyFinishedPrefixes(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == ASINFO){
        /* prefixesv4 */
        foreach(const QJsonValue &value, data["prefixesv4"].toArray()){
            QString Prefix = value.toObject()["prefix"].toString();
            QString shortname = value.toObject()["shortname"].toString();
            emit prefix(Prefix, shortname);
            log.resultsCount++;
        }
        /* prefixesv6 */
        foreach(const QJsonValue &value, data["prefixesv6"].toArray()){
            QString Prefix = value.toObject()["prefix"].toString();
            QString shortname = value.toObject()["shortname"].toString();
            emit prefix(Prefix, shortname);
            log.resultsCount++;
        }
    }

    end(reply);
}
