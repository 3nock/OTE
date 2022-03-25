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


SpyOnWeb::SpyOnWeb(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_SPYONWEB;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedRawJson);
    if(args.output_EnumNS)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedEnumNS);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedIp);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedSubdomain);
    if(args.output_HostnameIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SpyOnWeb::replyFinishedSubdomainIp);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_SPYONWEB).toString();
}
SpyOnWeb::~SpyOnWeb(){
    delete manager;
}

void SpyOnWeb::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
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

    if(args.input_IP){
        if(args.output_Hostname){
            url.setUrl("https://api.spyonweb.com/v1/ip/"+target+"?access_token="+m_key);
            request.setAttribute(QNetworkRequest::User, IP_API);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_Domain){
        if(args.output_IP || args.output_Hostname || args.output_HostnameIP){
            url.setUrl("https://api.spyonweb.com/v1/domain/"+target+"?access_token="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAIN_API);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    ///
    /// NS info ...
    ///

    if(args.output_EnumNS){
        url.setUrl("https://api.spyonweb.com/v1/dns_domain/"+target+"?access_token="+m_key);
        request.setAttribute(QNetworkRequest::User, DOMAINS_ON_NAMESERVER);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void SpyOnWeb::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_API:
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_API:
    {
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
        break;

    case IP_API:
    {
        QString ip = result["ip"].toObject().keys()[0];
        QStringList domains = result["ip"].toObject()[ip].toObject()["items"].toObject().keys();
        foreach(const QString &domain, domains){
            emit resultSubdomain(domain);
            log.resultsCount++;
        }
    }
    }

    end(reply);
}

void SpyOnWeb::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case DOMAIN_API:
    {
        QStringList addresses = result["ip_dns"].toObject().keys();
        foreach(const QString &address, addresses){
            emit resultIP(address);
            log.resultsCount++;
        }
    }
    }

    end(reply);
}

void SpyOnWeb::replyFinishedEnumNS(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();
    QJsonObject dns_domain = result["dns_domain"].toObject();
    QJsonObject nameserver = dns_domain.value(dns_domain.keys().at(0)).toObject();

    s3s_struct::NS ns;
    ns.ns = target;

    foreach(const QString domain, nameserver["items"].toObject().keys())
        ns.domains.insert(domain);

    emit resultEnumNS(ns);

    end(reply);
}
