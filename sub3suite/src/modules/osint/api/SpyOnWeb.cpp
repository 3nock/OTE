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
SpyOnWeb::SpyOnWeb(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "SpyOnWeb";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedRawJson);
    if(args.outputIp)
        connect(manager, &NetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedIp);
    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedSubdomain);
    if(args.outputSubdomainIp)
        connect(manager, &NetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedSubdomainIp);
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
    if(args.outputRaw){
        switch (args.rawOption) {
        case DOMAIN_API:
            url.setUrl("https://api.spyonweb.com/v1/domain/"+target+"?access_token="+m_key);
            break;
        case ADSENSE_API:
            url.setUrl("https://api.spyonweb.com/v1/adsense/"+target+"?access_token="+m_key);
            break;
        case ANALYTICS_API:
            url.setUrl("https://api.spyonweb.com/v1/analytics/"+target+"?access_token="+m_key);
            break;
        case IP_API:
            url.setUrl("https://api.spyonweb.com/v1/ip/"+target+"?access_token="+m_key);
            break;
        case DOMAINS_ON_NAMESERVER:
            url.setUrl("https://api.spyonweb.com/v1/dns_domain/"+target+"?access_token="+m_key);
            break;
        case NAMESERVERS_ON_IP:
            url.setUrl("https://api.spyonweb.com/v1/ip_dns/"+target+"?access_token="+m_key);
            break;
        case SUMMARY:
            url.setUrl("https://api.spyonweb.com/v1/summary/"+target+"?access_token="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        if(args.outputSubdomain){
            url.setUrl("https://api.spyonweb.com/v1/ip/"+target+"?access_token="+m_key);
            request.setAttribute(QNetworkRequest::User, IP_API);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args.inputDomain){
        if(args.outputIp || args.outputSubdomain || args.outputSubdomainIp){
            url.setUrl("https://api.spyonweb.com/v1/domain/"+target+"?access_token="+m_key);
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(QUERY_TYPE == DOMAIN_API){
        QString domainName = result["domain"].toObject().keys()[0];
        QJsonObject dns_servers = result["domain"].toObject()[domainName].toObject()["items"].toObject()["dns_servers"].toObject();
        QStringList dnsServers = dns_servers.keys();

        foreach(const QString &domain, dnsServers){
            QString address = dns_servers[domain].toString();
            emit resultSubdomainIp(domain, address);
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(QUERY_TYPE == DOMAIN_API){
        /* from dns_servers */
        QString domainName = result["domain"].toObject().keys()[0];
        QJsonObject dns_servers = result["domain"].toObject()[domainName].toObject()["items"].toObject()["dns_servers"].toObject();
        QStringList dnsServers = dns_servers.keys();
        foreach(const QString &domain, dnsServers){
            emit resultSubdomain(domain);
            log.resultsCount++;
        }

        /* from ip */
        QString ip = result["ip"].toObject().keys()[0];
        QStringList domains = result["ip"].toObject()[ip].toObject()["items"].toObject().keys();
        foreach(const QString &domain, domains){
            emit resultSubdomain(domain);
            log.resultsCount++;
        }
    }

    if(QUERY_TYPE == IP_API){
        QString ip = result["ip"].toObject().keys()[0];
        QStringList domains = result["ip"].toObject()[ip].toObject()["items"].toObject().keys();
        foreach(const QString &domain, domains){
            emit resultSubdomain(domain);
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(QUERY_TYPE == DOMAIN_API){
        QStringList addresses = result["ip_dns"].toObject().keys();
        foreach(const QString &address, addresses){
            emit resultIp(address);
            log.resultsCount++;
        }
    }

    end(reply);
}
