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

/* has a well detailed whois --> historical whois */
/* also nice resolution from ip -> subdomains */
/* has api v2 and v3 */
VirusTotal::VirusTotal(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "VirusTotal";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &VirusTotal::replyFinishedRaw);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &VirusTotal::replyFinishedIp);
    if(args->outputUrl)
        connect(manager, &MyNetworkAccessManager::finished, this, &VirusTotal::replyFinishedUrl);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &VirusTotal::replyFinishedSubdomain);
    if(args->outputSSLCert)
        connect(manager, &MyNetworkAccessManager::finished, this, &VirusTotal::replyFinishedSSLCert);
    ///
    /// obtain apikey...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("virustotal").toString();
    Config::generalConfig().endGroup();
}
VirusTotal::~VirusTotal(){
    delete manager;
}

void VirusTotal::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case DOMAIN_HISTORICAL_WHOIS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/historical_whois");
            break;
        case DOMAIN_HISTORICAL_SSL_CERTS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/historical_ssl_certificates");
            break;
        case DOMAIN_CNAME_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/cname_records");
            break;
        case DOMAIN_CAA_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/caa_records");
            break;
        case DOMAIN_NS_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/ns_records");
            break;
        case DOMAIN_MX_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/mx_records");
            break;
        case DOMAIN_PARENT:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/parent");
            break;
        case DOMAIN_RESOLUTIONS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/resolutions");
            break;
        case DOMAIN_SIBLINGS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/siblings");
            break;
        case DOMAIN_SOA_RECORDS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/soa_records");
            break;
        case DOMAIN_SUBDOMAINS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/subdomains");
            break;
        case DOMAIN_URLS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/urls");
            break;
        case IP_HISTORICAL_SSL_CERTS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target+"/historical_ssl_certificates");
            break;
        case IP_HISTORICAL_WHOIS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target+"/historical_whois");
            break;
        case IP_RESOLUTIONS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target+"/resolutions");
            break;
        case IP_URLS:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target+"/urls");
            break;
        case V2_DOMAIN:
            url.setUrl("https://www.virustotal.com/vtapi/v2/domain/report?apikey="+m_key+"&domain="+args->target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        case V2_IPADDRESS:
            url.setUrl("https://www.virustotal.com/vtapi/v2/ip-address/report?apikey="+m_key+"&ip="+args->target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        request.setRawHeader("x-apikey", m_key.toUtf8());
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputIp || args->outputSubdomain || args->outputUrl){
            url.setUrl("https://www.virustotal.com/vtapi/v2/domain/report?apikey="+m_key+"&domain="+args->target);
            request.setAttribute(QNetworkRequest::User, V2_DOMAIN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        //...
        request.setRawHeader("x-apikey", m_key.toUtf8());
        //...
        if(args->outputSubdomain || args->outputSSLCert){
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/historical_ssl_certificates");
            request.setAttribute(QNetworkRequest::User, DOMAIN_HISTORICAL_SSL_CERTS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputIp){
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/resolutions");
            request.setAttribute(QNetworkRequest::User, DOMAIN_RESOLUTIONS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputIp || args->outputSubdomain || args->outputSSLCert){
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target+"/subdomains");
            request.setAttribute(QNetworkRequest::User, DOMAIN_SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args->inputIp){
        if(args->outputIp || args->outputSubdomain || args->outputUrl){
            url.setUrl("https://www.virustotal.com/vtapi/v2/ip-address/report?apikey="+m_key+"&ip="+args->target);
            request.setAttribute(QNetworkRequest::User, V2_IPADDRESS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        //...
        request.setRawHeader("x-apikey", m_key.toUtf8());
        //...
        if(args->outputSubdomain || args->outputSSLCert){
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target+"/historical_ssl_certificates");
            request.setAttribute(QNetworkRequest::User, IP_HISTORICAL_SSL_CERTS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputSubdomain){
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target+"/resolutions");
            request.setAttribute(QNetworkRequest::User, IP_RESOLUTIONS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void VirusTotal::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    /* from v2 api endpoint */
    if(requestType == V2_DOMAIN || requestType == V2_IPADDRESS){
        QJsonArray subdomains = document.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomains){
            QString hostname = value.toString();
            emit subdomain(hostname);
            log.resultsCount++;
        }
    }

    /* alternative names from historical ssl certificates */
    if(requestType == DOMAIN_HISTORICAL_SSL_CERTS || requestType == IP_HISTORICAL_SSL_CERTS){
        foreach(const QJsonValue &value, data){
            QJsonArray subject_alternative_name = value.toObject()["attributes"].toObject()["extensions"].toObject()["subject_alternative_name"].toArray();
            foreach(const QJsonValue &alt_name, subject_alternative_name){
                QString hostname = alt_name.toString();
                emit subdomain(hostname);
                log.resultsCount++;
            }
        }
    }

    if(requestType == DOMAIN_SUBDOMAINS){
        foreach(const QJsonValue &value, data){
           /* from id */
           QString hostname = value.toObject()["id"].toString();
           emit subdomain(hostname);
           log.resultsCount++;

           /* from last dns records */
           QJsonArray last_dns_records = value.toObject()["attributes"].toObject()["last_dns_records"].toArray();
           foreach(const QJsonValue &dnsRecord, last_dns_records){
               QString type = dnsRecord.toObject()["type"].toString();

               if(type == "NS"){
                   QString hostname = dnsRecord.toObject()["value"].toString();
                   emit NS(hostname);
                   log.resultsCount++;
               }
               if(type == "MX"){
                   QString hostname = dnsRecord.toObject()["value"].toString();
                   emit MX(hostname);
                   log.resultsCount++;
               }
               if(type == "CNAME"){
                   QString hostname = dnsRecord.toObject()["value"].toString();
                   emit CNAME(hostname);
                   log.resultsCount++;
               }
           }

           /* from ssl cert alternative name */
           QJsonObject last_https_certificate = value.toObject()["attributes"].toObject()["last_https_certificate"].toObject();
           QJsonArray subject_alternative_name = last_https_certificate["extensions"].toObject()["subject_alternative_name"].toArray();
           foreach(const QJsonValue &altName, subject_alternative_name){
               QString hostname = altName.toString();
               emit subdomain(hostname);
               log.resultsCount++;
           }
        }
    }

    if(requestType == IP_RESOLUTIONS){
        foreach(const QJsonValue &value, data){
            QString hostname = value.toObject()["attributes"].toObject()["host_name"].toString();
            emit subdomain(hostname);
            log.resultsCount++;
        }
    }
    end(reply);
}

void VirusTotal::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    /* from v2 api endpoint */
    if(requestType == V2_DOMAIN || requestType == V2_IPADDRESS){
        QJsonArray resolutions = document.object()["resolutions"].toArray();
        foreach(const QJsonValue &value, resolutions){
            QString address = value.toObject()["ip_address"].toString();
            emit ip(address);
            log.resultsCount++;
        }
    }

    /* from v3 api endpoint */
    if(requestType == DOMAIN_RESOLUTIONS){
        foreach(const QJsonValue &value, data){
            QString address = value.toObject()["attributes"].toObject()["ip_address"].toString();
            emit ip(address);
            log.resultsCount++;
        }
    }

    if(requestType == DOMAIN_SUBDOMAINS){
        foreach(const QJsonValue &value, data){
           /* from last dns records */
           QJsonArray last_dns_records = value.toObject()["attributes"].toObject()["last_dns_records"].toArray();
           foreach(const QJsonValue &dnsRecord, last_dns_records){
               QString type = dnsRecord.toObject()["type"].toString();

               if(type == "A"){
                   QString address = dnsRecord.toObject()["value"].toString();
                   emit ipA(address);
                   log.resultsCount++;
               }
               if(type == "AAAA"){
                   QString address = dnsRecord.toObject()["value"].toString();
                   emit ipAAAA(address);
                   log.resultsCount++;
               }
           }
        }
    }
    end(reply);
}

void VirusTotal::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    /* from v2 api endpoint */
    if(requestType == V2_DOMAIN || requestType == V2_IPADDRESS){
        QJsonArray detected_urls = document.object()["detected_urls"].toArray();
        foreach(const QJsonValue &value, detected_urls){
            QString detected_url = value.toArray().at(0).toString();
            emit url(detected_url);
            log.resultsCount++;
        }

        QJsonArray undetected_urls = document.object()["undetected_urls"].toArray();
        foreach(const QJsonValue &value, undetected_urls){
            QString undetected_url = value.toArray().at(0).toString();
            emit url(undetected_url);
            log.resultsCount++;
        }
    }
    end(reply);
}

void VirusTotal::replyFinishedSSLCert(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    /* certificate id from historical ssl certificates */
    if(requestType == DOMAIN_HISTORICAL_SSL_CERTS || requestType == IP_HISTORICAL_SSL_CERTS){
        foreach(const QJsonValue &value, data){
            QString cert_id = value.toObject()["id"].toString();
            emit sslCert(cert_id);
            log.resultsCount++;
        }
    }

    if(requestType == DOMAIN_SUBDOMAINS){
        foreach(const QJsonValue &value, data){
           /* from ssl cert alternative name */
           QJsonObject last_https_certificate = value.toObject()["attributes"].toObject()["last_https_certificate"].toObject();
           QString thumbprint = last_https_certificate["thumbprint"].toString();
           emit sslCert(thumbprint);
           log.resultsCount++;
        }
    }
    end(reply);
}
