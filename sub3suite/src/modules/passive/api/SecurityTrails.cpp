#include "SecurityTrails.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define COMPANY_ASSOCIATED_IP 0
#define COMPANY_DETAILS 1
#define DOMAIN_ASSOCIATED_DOMAINS 2
#define DOMAIN_DETAILS 3
#define DOMAIN_SUBDOMAIN 4
#define DOMAIN_SSL_CERT 5
#define DOMAIN_WHOIS 6
#define GENERAL_PING 7
#define GENERAL_USAGE 8
#define HISTORY_DNS 9
#define HISTORY_WHOIS 10
#define IP_NEIGHBOURS 11
#define IP_WHOIS 12

/*
 * Limited to 2000 results, 50 queries a month for the Free plan and to 10000 for all paid subscriptions.
 */
SecurityTrails::SecurityTrails(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_SECURITYTRAILS;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SecurityTrails::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SecurityTrails::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SecurityTrails::replyFinishedIp);
    if(args.outputCidr)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SecurityTrails::replyFinishedCidr);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_SECURITYTRAILS).toString();
    
}
SecurityTrails::~SecurityTrails(){
    delete manager;
}

void SecurityTrails::start(){
    QNetworkRequest request;
    request.setRawHeader("APIKEY", m_key.toUtf8());
    request.setRawHeader("Accept", "application/json");

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case HISTORY_DNS:
            url.setUrl("https://api.securitytrails.com/v1/history/"+target+"/dns/any");
            break;
        case DOMAIN_SUBDOMAIN:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target+"/subdomains?children_only=false&include_inactive=true");
            break;
        case HISTORY_WHOIS:
            url.setUrl("https://api.securitytrails.com/v1/history/"+target+"/whois");
            break;
        case DOMAIN_WHOIS:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target+"/whois");
            break;
        case DOMAIN_ASSOCIATED_DOMAINS:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target+"/associated");
            break;
        case DOMAIN_DETAILS:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target);
            break;
        case COMPANY_DETAILS:
            url.setUrl("https://api.securitytrails.com/v1/company/"+target);
            break;
        case COMPANY_ASSOCIATED_IP:
            url.setUrl("https://api.securitytrails.com/v1/company/"+target+"/associated-ips");
            break;
        case DOMAIN_SSL_CERT:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target+"/ssl?include_subdomains=false&status=valid");
            break;
        case IP_NEIGHBOURS:
            url.setUrl("https://api.securitytrails.com/v1/ips/nearby/"+target);
            break;
        case IP_WHOIS:
            url.setUrl("https://api.securitytrails.com/v1/ips/"+target+"/whois");
            break;
        case GENERAL_PING:
            url.setUrl("https://api.securitytrails.com/v1/ping");
            break;
        case GENERAL_USAGE:
            url.setUrl("https://api.securitytrails.com/v1/account/usage");
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomain){
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target+"/subdomains?children_only=false&include_inactive=true");
            request.setAttribute(QNetworkRequest::User, DOMAIN_SUBDOMAIN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputIp){
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target);
            url.setUrl("https://api.securitytrails.com/v1/domain/"+target+"/subdomains?children_only=false&include_inactive=true");
            request.setAttribute(QNetworkRequest::User, DOMAIN_DETAILS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputCidr){
            url.setUrl("https://api.securitytrails.com/v1/company/"+target+"/associated-ips");
            request.setAttribute(QNetworkRequest::User, COMPANY_ASSOCIATED_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputIp){
        if(args.outputSubdomain){
            url.setUrl("https://api.securitytrails.com/v1/ips/nearby/"+target);
            request.setAttribute(QNetworkRequest::User, IP_NEIGHBOURS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void SecurityTrails::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case DOMAIN_SUBDOMAIN:
    {
        QJsonArray subdomainList = document.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomainList){
            emit resultSubdomain(value.toString().append(".").append(target));
            log.resultsCount++;
        }
    }
        break;

    case IP_NEIGHBOURS:
        foreach(const QJsonValue &value, document.object()["blocks"].toArray()){
            QString ipCidr = value.toObject()["ip"].toString();
            QJsonArray hostnames = value.toObject()["hostnames"].toArray();

            foreach(const QJsonValue &value, hostnames){
                emit resultSubdomain(value.toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void SecurityTrails::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_DETAILS:
        QJsonObject a = document.object()["current_dns"].toObject()["a"].toObject();
        QJsonObject aaaa = document.object()["current_dns"].toObject()["aaaa"].toObject();

        if(!a.isEmpty()){
            foreach(const QJsonValue &value, a["values"].toArray()){
                emit resultA(value.toObject()["ip"].toString());
                log.resultsCount++;
            }
        }
        if(!aaaa.isEmpty()){
            foreach(const QJsonValue &value, aaaa["values"].toArray()){
                emit resultAAAA(value.toObject()["ip"].toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void SecurityTrails::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case COMPANY_ASSOCIATED_IP:
        foreach(const QJsonValue &value, document.object()["records"].toArray()){
            emit resultCIDR(value.toObject()["cidr"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
