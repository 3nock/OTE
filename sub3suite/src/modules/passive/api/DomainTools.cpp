#include "DomainTools.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define ACCOUNT_INFO 0
#define BRAND_MONITOR 1
#define DOMAIN_PROFILE 2
#define DOMAIN_REPUTATION 3
#define DOMAIN_RISK_SCORE 4
#define DOMAIN_SEARCH 5
#define HOSTING_HISTORY 6
#define IP_MONITOR 7
#define IP_REGISTRANT_MONITOR 8
#define IRIS_ENRICH 9
#define IRIS_INVESTIGATE_DOMAIN 10
#define IRIS_INVESTIGATE_IP 11
#define IRIS_PIVOT_DOMAIN 12
#define IRIS_PIVOT_IP 13
#define NAMESERVER_MONITOR 14
#define PARSED_WHOIS 15
#define PHISHEYE 16
#define REGISTRANT_MONITOR 17
#define REVERSE_IP 18
#define REVERSE_IP_WHOIS 19
#define REVERSE_MX_DOMAIN 20
#define REVERSE_MX_IP 21
#define REVERSE_MX_MX 22
#define REVERSE_NAMESERVER 23
#define REVERSE_WHOIS 24
#define WHOIS_HISTORY 25
#define WHOIS_LOOKUP 26


DomainTools::DomainTools(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_DOMAINTOOLS;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DomainTools::replyFinishedRawJson);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DomainTools::replyFinishedIp);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DomainTools::replyFinishedSubdomain);
    if(args.output_HostnameIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DomainTools::replyFinishedSubdomainIp);

    /* getting api key... */
    m_key = APIKEY.value("domaintools_key").toString();
    m_username = APIKEY.value("domaintools_username").toString();
    
}
DomainTools::~DomainTools(){
    delete manager;
}

void DomainTools::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case BRAND_MONITOR:
            url.setUrl("https://api.domaintools.com/v1/account/?api_username="+m_username+"&api_key="+m_key);
            break;
        case ACCOUNT_INFO:
            url.setUrl("https://api.domaintools.com/v1/mark-alert/?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case DOMAIN_PROFILE:
            url.setUrl("https://api.domaintools.com/v1/"+target+"?api_username="+m_username+"&api_key="+m_key);
            break;
        case DOMAIN_REPUTATION:
            url.setUrl("https://api.domaintools.com/v1/reputation/?domain="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case DOMAIN_RISK_SCORE:
            url.setUrl("https://api.domaintools.com/v1/risk/evidence/?domain="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case DOMAIN_SEARCH:
            url.setUrl("https://api.domaintools.com/v2/domain-search/?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case HOSTING_HISTORY:
            url.setUrl("https://api.domaintools.com/v1/"+target+"/hosting-history/?api_username="+m_username+"&api_key="+m_key);
            break;
        case IP_MONITOR:
            url.setUrl("https://api.domaintools.com/v1/ip-monitor/?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case IP_REGISTRANT_MONITOR:
            url.setUrl("https://api.domaintools.com/v1/ip-registrant-monitor/?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case IRIS_ENRICH:
            url.setUrl("https://api.domaintools.com/v1/iris-enrich/?domain="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case IRIS_INVESTIGATE_DOMAIN:
            url.setUrl("https://api.domaintools.com/v1/iris-investigate/?domain="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case IRIS_INVESTIGATE_IP:
            url.setUrl("https://api.domaintools.com/v1/iris-investigate/?ip="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case IRIS_PIVOT_DOMAIN:
            url.setUrl("https://api.domaintools.com/v1/iris/?domain="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case IRIS_PIVOT_IP:
            url.setUrl("https://api.domaintools.com/v1/iris/?ip="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case NAMESERVER_MONITOR:
            url.setUrl("https://api.domaintools.com/v1/name-server-monitor/?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case PARSED_WHOIS:
            url.setUrl("https://api.domaintools.com/v1/"+target+"/whois/parsed?api_username="+m_username+"&api_key="+m_key);
            break;
        case REGISTRANT_MONITOR:
            url.setUrl("https://api.domaintools.com/v1/registrant-alert/?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case PHISHEYE:
            url.setUrl("https://api.domaintools.com/v1/phisheye?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case REVERSE_IP:
            url.setUrl("https://api.domaintools.com/v1/"+target+"/host-domains/?api_username="+m_username+"&api_key="+m_key);
            break;
        case REVERSE_IP_WHOIS:
            url.setUrl("https://api.domaintools.com/v1/reverse-ip-whois/?ip="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case REVERSE_MX_DOMAIN:
            url.setUrl("https://api.domaintools.com/v1/reverse-mx/?domain="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case REVERSE_MX_IP:
            url.setUrl("https://api.domaintools.com/v1/reverse-mx/?mx_ip="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case REVERSE_MX_MX:
            url.setUrl("https://api.domaintools.com/v1/reverse-mx/?mx_server="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case REVERSE_NAMESERVER:
            url.setUrl("https://api.domaintools.com/v1/"+target+"/name-server-domains/?api_username="+m_username+"&api_key="+m_key);
            break;
        case REVERSE_WHOIS:
            url.setUrl("https://api.domaintools.com/v1/reverse-whois/?terms="+target+"&api_username="+m_username+"&api_key="+m_key);
            break;
        case WHOIS_HISTORY:
            url.setUrl("https://api.domaintools.com/v1/"+target+"/whois/history/?sort=date_asc&api_username="+m_username+"&api_key="+m_key);
            break;
        case WHOIS_LOOKUP:
            url.setUrl("https://api.domaintools.com/v1/"+target+"/whois/?api_username="+m_username+"&api_key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_IP || args.output_Hostname || args.output_HostnameIP){
            url.setUrl("https://api.domaintools.com/v1/"+target+"/reverse-ip/?api_username="+m_username+"&api_key="+m_key);
            request.setAttribute(QNetworkRequest::User, REVERSE_IP);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_IP){
        if(args.output_IP || args.output_Hostname || args.output_HostnameIP){
            url.setUrl("https://api.domaintools.com/v1/"+target+"/host-domains/?api_username="+m_username+"&api_key="+m_key);
            request.setAttribute(QNetworkRequest::User, REVERSE_IP);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Search){
        if(args.output_Hostname){
            url.setUrl("https://api.domaintools.com/v2/domain-search/?query="+target+"&api_username="+m_username+"&api_key="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAIN_SEARCH);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    ///
    /// For Enums...
    ///

    if(args.output_EnumMX){
        url.setUrl("https://api.domaintools.com/v1/reverse-mx/?mx_server="+target+"&api_username="+m_username+"&api_key="+m_key);
        request.setAttribute(QNetworkRequest::User, REVERSE_MX_MX);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumNS){
        url.setUrl("https://api.domaintools.com/v1/"+target+"/name-server-domains/?api_username="+m_username+"&api_key="+m_key);
        request.setAttribute(QNetworkRequest::User, REVERSE_NAMESERVER);
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

void DomainTools::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case REVERSE_IP:
        foreach(const QJsonValue &ip_address, response["ip_addresses"].toArray()){
            QString ip = ip_address.toObject()["ip_address"].toString();
            foreach(const QJsonValue &domain, ip_address.toObject()["domain_names"].toArray()){
                emit resultSubdomainIp(domain.toString(), ip);
                log.resultsCount++;
            }
        }
        break;
    }

    this->end(reply);
}

void DomainTools::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    switch(reply->property(REQUEST_TYPE).toInt())
    {
    case REVERSE_IP:
        foreach(const QJsonValue &ip_address, response["ip_addresses"].toArray()){
            foreach(const QJsonValue &domain, ip_address.toObject()["domain_names"].toArray()){
                emit resultSubdomain(domain.toString());
                log.resultsCount++;
            }
        }
        break;

    case DOMAIN_SEARCH:
        foreach(const QJsonValue &results, response["results"].toArray()){
            QString sld = results.toObject()["sld"].toString();
            foreach(const QJsonValue &tld, results["tlds"].toArray()){
                QString domain = sld.append(".").append(tld.toString());
                emit resultSubdomain(domain);
                log.resultsCount++;
            }
        }
        break;
    }

    this->end(reply);
}

void DomainTools::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case REVERSE_IP:
        foreach(const QJsonValue &ip_address, response["ip_addresses"].toArray()){
            emit resultIP(ip_address.toObject()["ip_address"].toString());
            log.resultsCount++;
        }
        break;
    }

    this->end(reply);
}

void DomainTools::replyFinishedEnumMX(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    s3s_struct::MX mx;
    mx.mx = target;

    foreach(const QJsonValue &domain, response["domains"].toArray())
        mx.domains.insert(domain.toString());

    foreach(const QJsonValue &ip, response["ips"].toArray())
        mx.ip.insert(ip.toString());
    /*
    foreach(const QJsonValue &mx_server, response["mx_servers"].toArray())
        mx.mx_servers.insert(mx_server.toString());
    */

    if(!(mx.domains.isEmpty() && mx.ip.isEmpty()))
        emit resultEnumMX(mx);

    this->end(reply);
}

void DomainTools::replyFinishedEnumNS(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    s3s_struct::NS ns;
    ns.ns = target;

    foreach(const QJsonValue &domain, response["primary_domains"].toArray())
        ns.domains.insert(domain.toString());
    foreach(const QJsonValue &domain, response["secondary_domains"].toArray())
        ns.domains.insert(domain.toString());

    if(!ns.domains.isEmpty())
        emit resultEnumNS(ns);

    this->end(reply);
}
