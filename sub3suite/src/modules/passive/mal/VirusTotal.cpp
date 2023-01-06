#include "VirusTotal.h"
#include "src/utils/Config.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DOMAIN_CAA_RECORDS 0
#define DOMAIN_CNAME_RECORDS 1
#define DOMAIN_HISTORICAL_SSL_CERTS 2
#define DOMAIN_HISTORICAL_WHOIS 3
#define DOMAIN_MX_RECORDS 4
#define DOMAIN_NS_RECORDS 5
#define DOMAIN_PARENT 6
#define DOMAIN_RESOLUTIONS 7
#define DOMAIN_SIBLINGS 8
#define DOMAIN_SOA_RECORDS 9
#define DOMAIN_SUBDOMAINS 10
#define DOMAIN_URLS 11
#define IP_HISTORICAL_SSL_CERTS 12
#define IP_HISTORICAL_WHOIS 13
#define IP_RESOLUTIONS 14
#define IP_URLS 15
#define V2_DOMAIN 16
#define V2_IPADDRESS 17

/*
 * has api v2 and v3
 */
VirusTotal::VirusTotal(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_VIRUSTOTAL;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &VirusTotal::replyFinishedRawJson);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &VirusTotal::replyFinishedIp);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &VirusTotal::replyFinishedUrl);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &VirusTotal::replyFinishedSubdomain);
    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &VirusTotal::replyFinishedSSL);

    /* obtain apikey */
    m_key = gConfig.keys.VirusTotal;
}
VirusTotal::~VirusTotal(){
    delete manager;
}

void VirusTotal::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case DOMAIN_HISTORICAL_WHOIS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/historical_whois");
            break;
        case DOMAIN_HISTORICAL_SSL_CERTS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/historical_ssl_certificates");
            break;
        case DOMAIN_CNAME_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/cname_records");
            break;
        case DOMAIN_CAA_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/caa_records");
            break;
        case DOMAIN_NS_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/ns_records");
            break;
        case DOMAIN_MX_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/mx_records");
            break;
        case DOMAIN_PARENT:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/parent");
            break;
        case DOMAIN_RESOLUTIONS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/resolutions");
            break;
        case DOMAIN_SIBLINGS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/siblings");
            break;
        case DOMAIN_SOA_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/soa_records");
            break;
        case DOMAIN_SUBDOMAINS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/subdomains");
            break;
        case DOMAIN_URLS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/urls");
            break;
        case IP_HISTORICAL_SSL_CERTS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+target+"/historical_ssl_certificates");
            break;
        case IP_HISTORICAL_WHOIS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+target+"/historical_whois");
            break;
        case IP_RESOLUTIONS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+target+"/resolutions");
            break;
        case IP_URLS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+target+"/urls");
            break;
        case V2_DOMAIN:
            url.setUrl("https://www.virustotal.com/vtapi/v2/domain/report?apikey="+m_key+"&domain="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        case V2_IPADDRESS:
            url.setUrl("https://www.virustotal.com/vtapi/v2/ip-address/report?apikey="+m_key+"&ip="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        request.setRawHeader("x-apikey", m_key.toUtf8());
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_IP || args.output_Hostname || args.output_URL){
            url.setUrl("https://www.virustotal.com/vtapi/v2/domain/report?apikey="+m_key+"&domain="+target);
            request.setAttribute(QNetworkRequest::User, V2_DOMAIN);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_SSL){
            request.setRawHeader("x-apikey", m_key.toUtf8());
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+target+"/historical_ssl_certificates");
            request.setAttribute(QNetworkRequest::User, DOMAIN_HISTORICAL_SSL_CERTS);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_IP){
        if(args.output_IP || args.output_Hostname || args.output_URL){
            url.setUrl("https://www.virustotal.com/vtapi/v2/ip-address/report?apikey="+m_key+"&ip="+target);
            request.setAttribute(QNetworkRequest::User, V2_IPADDRESS);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_SSL){
            request.setRawHeader("x-apikey", m_key.toUtf8());
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+target+"/historical_ssl_certificates");
            request.setAttribute(QNetworkRequest::User, IP_HISTORICAL_SSL_CERTS);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void VirusTotal::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case V2_DOMAIN:
    case V2_IPADDRESS:
        QJsonArray subdomains = document.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomains){
            QString hostname = value.toString();
            emit resultSubdomain(hostname);
            log.resultsCount++;
        }
        break;
    }

    this->end(reply);
}

void VirusTotal::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case V2_DOMAIN:
    case V2_IPADDRESS:
        QJsonArray resolutions = document.object()["resolutions"].toArray();
        foreach(const QJsonValue &value, resolutions){
            QString address = value.toObject()["ip_address"].toString();
            emit resultIP(address);
            log.resultsCount++;
        }
        break;
    }

    this->end(reply);
}

void VirusTotal::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case V2_DOMAIN:
    case V2_IPADDRESS:
        QJsonArray detected_urls = document.object()["detected_urls"].toArray();
        foreach(const QJsonValue &value, detected_urls){
            QString detected_url = value.toArray().at(0).toString();
            emit resultURL(detected_url);
            log.resultsCount++;
        }

        QJsonArray undetected_urls = document.object()["undetected_urls"].toArray();
        foreach(const QJsonValue &value, undetected_urls){
            QString undetected_url = value.toArray().at(0).toString();
            emit resultURL(undetected_url);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void VirusTotal::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_HISTORICAL_SSL_CERTS:
    case IP_HISTORICAL_SSL_CERTS:
        foreach(const QJsonValue &value, data){
            QString cert_id = value.toObject()["id"].toString();
            emit resultSSL(cert_id);
            log.resultsCount++;
        }
        break;
    }

    this->end(reply);
}
