#include "SpyOnWeb.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ADSENSE_API 0
#define ANALYTICS_API 1
#define DOMAIN_API 2
#define DOMAINS_ON_NAMESERVER 3
#define IP_API 4
#define NAMESERVERS_ON_IP 5
#define SUMMARY 6

/*
 * resolves the domains on nameserver...
 */
SpyOnWeb::SpyOnWeb(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "SpyOnWeb";

    if(args->outputRaw)
        connect(manager, &MyNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedRawJson);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedSubdomain);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedSubdomainIp);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("spyonweb").toString();
    Config::generalConfig().endGroup();
}
SpyOnWeb::~SpyOnWeb(){
    delete manager;
}

void SpyOnWeb::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case DOMAIN_API:
            url.setUrl("https://api.spyonweb.com/v1/domain/"+args->target+"?access_token="+m_key);
            break;
        case ADSENSE_API:
            url.setUrl("https://api.spyonweb.com/v1/adsense/"+args->target+"?access_token="+m_key);
            break;
        case ANALYTICS_API:
            url.setUrl("https://api.spyonweb.com/v1/analytics/"+args->target+"?access_token="+m_key);
            break;
        case IP_API:
            url.setUrl("https://api.spyonweb.com/v1/ip/"+args->target+"?access_token="+m_key);
            break;
        case DOMAINS_ON_NAMESERVER:
            url.setUrl("https://api.spyonweb.com/v1/dns_domain/"+args->target+"?access_token="+m_key);
            break;
        case NAMESERVERS_ON_IP:
            url.setUrl("https://api.spyonweb.com/v1/ip_dns/"+args->target+"?access_token="+m_key);
            break;
        case SUMMARY:
            url.setUrl("https://api.spyonweb.com/v1/summary/"+args->target+"?access_token="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputIp){
        if(args->outputSubdomain){
            url.setUrl("https://api.spyonweb.com/v1/ip/"+args->target+"?access_token="+m_key);
            request.setAttribute(QNetworkRequest::User, IP_API);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputDomain){
        if(args->outputIp || args->outputSubdomain || args->outputSubdomainIp){
            url.setUrl("https://api.spyonweb.com/v1/domain/"+args->target+"?access_token="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAIN_API);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void SpyOnWeb::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(requestType == DOMAIN_API){
        QString domainName = result["domain"].toObject().keys()[0];
        QJsonObject dns_servers = result["domain"].toObject()[domainName].toObject()["items"].toObject()["dns_servers"].toObject();
        QStringList dnsServers = dns_servers.keys();

        foreach(const QString &domain, dnsServers){
            QString address = dns_servers[domain].toString();
            emit subdomainIp(domain, address);
            log.resultsCount++;
        }
    }

    end(reply);
}

void SpyOnWeb::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(requestType == DOMAIN_API){
        /* from dns_servers */
        QString domainName = result["domain"].toObject().keys()[0];
        QJsonObject dns_servers = result["domain"].toObject()[domainName].toObject()["items"].toObject()["dns_servers"].toObject();
        QStringList dnsServers = dns_servers.keys();
        foreach(const QString &domain, dnsServers){
            emit subdomain(domain);
            log.resultsCount++;
        }

        /* from ip */
        QString ip = result["ip"].toObject().keys()[0];
        QStringList domains = result["ip"].toObject()[ip].toObject()["items"].toObject().keys();
        foreach(const QString &domain, domains){
            emit subdomain(domain);
            log.resultsCount++;
        }
    }

    if(requestType == IP_API){
        QString ip = result["ip"].toObject().keys()[0];
        QStringList domains = result["ip"].toObject()[ip].toObject()["items"].toObject().keys();
        foreach(const QString &domain, domains){
            emit subdomain(domain);
            log.resultsCount++;
        }
    }

    end(reply);
}

void SpyOnWeb::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(requestType == DOMAIN_API){
        QStringList addresses = result["ip_dns"].toObject().keys();
        foreach(const QString &address, addresses){
            emit ip(address);
            log.resultsCount++;
        }
    }

    end(reply);
}
