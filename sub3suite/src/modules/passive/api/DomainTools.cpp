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
#define REVERSE_NAMESERVER 20
#define REVERSE_WHOIS 21
#define WHOIS_HISTORY 22
#define WHOIS_LOOKUP 23


DomainTools::DomainTools(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_DOMAINTOOLS;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DomainTools::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DomainTools::replyFinishedSubdomain);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DomainTools::replyFinishedSubdomainIp);

    /* getting api key... */
    
    m_key = APIKEY.value("domaintools_key").toString();
    m_username = APIKEY.value("domaintools_username").toString();
    
}
DomainTools::~DomainTools(){
    delete manager;
}

void DomainTools::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
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
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputIp){
            url.setUrl("https://api.domaintools.com/v1/"+target+"/hosting-history/");
            request.setAttribute(QNetworkRequest::User, HOSTING_HISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputIp){
        if(args.outputSubdomain){
            url.setUrl("https://api.domaintools.com/v1/"+target+"/host-domains/");
            request.setAttribute(QNetworkRequest::User, REVERSE_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }
}

void DomainTools::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    if(QUERY_TYPE == REVERSE_IP){
        foreach(const QJsonValue &ip_address, response["ip_addresses"].toArray()){
            QString ip = ip_address.toObject()["ip_address"].toString();
            foreach(const QJsonValue &domain, ip_address.toObject()["domain_names"].toArray()){
                emit resultSubdomainIp(domain.toString(), ip);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void DomainTools::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();


    if(QUERY_TYPE == HOSTING_HISTORY){
        foreach(const QJsonValue &value, response["ip_history"].toArray()){
            QJsonObject ip_history = value.toObject();
            if(!ip_history["post_ip"].isNull() || !ip_history["post_ip"].toString().isEmpty()){
                QString post_ip = value.toObject()["post_ip"].toString();
                emit resultIp(post_ip);
                log.resultsCount++;
            }
            if(!ip_history["pre_ip"].isNull() || !ip_history["pre_ip"].toString().isEmpty()){
                QString pre_ip = value.toObject()["pre_ip"].toString();
                emit resultIp(pre_ip);
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == REVERSE_IP){
        foreach(const QJsonValue &ip_address, response["ip_addresses"].toArray()){
            foreach(const QJsonValue &domain, ip_address.toObject()["domain_names"].toArray()){
                emit resultSubdomain(domain.toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}
