#include "WebResolver.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DISPOSABLE_MAIL_CHECK 0
#define DNSRESOLVER 1
#define DOMAIN_INFO 2
#define GEOIP 3
#define IP_TO_WEBSITE 4
#define PHONENUMBER_CHECK 5
#define PORTSCAN 6
#define SCREENSHOT_TOOL 7
#define WEBSITE_HEADERS 8
#define WEBSITE_WHOIS 9


WebResolver::WebResolver(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "WebResolver";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WebResolver::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WebResolver::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WebResolver::replyFinishedIp);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &WebResolver::replyFinishedSubdomainIp);
    ///
    /// getting api key...
    ///
    
    m_key = APIKEY.value("webresolver").toString();
    
}
WebResolver::~WebResolver(){
    delete manager;
}

void WebResolver::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case GEOIP:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=geoip&string="+target);
            break;
        case DNSRESOLVER:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=dns&string="+target);
            break;
        case PHONENUMBER_CHECK:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=phonenumbercheck&string="+target);
            break;
        case SCREENSHOT_TOOL:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=screenshot&string="+target);
            break;
        case WEBSITE_HEADERS:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&html=0&action=header&string="+target);
            break;
        case WEBSITE_WHOIS:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&html=0&action=whois&string="+target);
            break;
        case PORTSCAN:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=portscan&string="+target);
            break;
        case DISPOSABLE_MAIL_CHECK:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=disposable_email&string="+target);
            break;
        case IP_TO_WEBSITE:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=ip2websites&string="+target);
            break;
        case DOMAIN_INFO:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=domaininfo&string="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=dns&string="+target);
        request.setAttribute(QNetworkRequest::User, DNSRESOLVER);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void WebResolver::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(QUERY_TYPE == DNSRESOLVER){
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString type = record.toObject()["type"].toString();

            if(type == "MX" || type == "NS" || type == "CNAME"){
                QString hostname = record.toObject()["server"].toString();
                QString address = record.toObject()["ip"].toString();
                emit resultSubdomainIp(hostname, address);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void WebResolver::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(QUERY_TYPE == DNSRESOLVER){
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString address = record.toObject()["ip"].toString();
            emit resultIp(address);
            log.resultsCount++;
        }
    }
    end(reply);
}

void WebResolver::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(QUERY_TYPE == DNSRESOLVER){
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString type = record.toObject()["type"].toString();

            if(type == "MX"){
                QString hostname = record.toObject()["server"].toString();
                emit resultMX(hostname);
                log.resultsCount++;
            }
            if(type == "NS"){
                QString hostname = record.toObject()["server"].toString();
                emit resultNS(hostname);
                log.resultsCount++;
            }
            if(type == "CNAME"){
                QString hostname = record.toObject()["server"].toString();
                emit resultCNAME(hostname);
                log.resultsCount++;
            }
            if(type == "TXT"){
                QString text = record.toObject()["text"].toString();
                emit resultTXT(text);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}
