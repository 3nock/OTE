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


WebResolver::WebResolver(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "WebResolver";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &WebResolver::replyFinishedRawJson);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &WebResolver::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &NetworkAccessManager::finished, this, &WebResolver::replyFinishedIp);
    if(args->outputSubdomainIp)
        connect(manager, &NetworkAccessManager::finished, this, &WebResolver::replyFinishedSubdomainIp);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("webresolver").toString();
    Config::generalConfig().endGroup();
}
WebResolver::~WebResolver(){
    delete manager;
}

void WebResolver::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case GEOIP:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=geoip&string="+args->target);
            break;
        case DNSRESOLVER:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=dns&string="+args->target);
            break;
        case PHONENUMBER_CHECK:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=phonenumbercheck&string="+args->target);
            break;
        case SCREENSHOT_TOOL:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=screenshot&string="+args->target);
            break;
        case WEBSITE_HEADERS:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&html=0&action=header&string="+args->target);
            break;
        case WEBSITE_WHOIS:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&html=0&action=whois&string="+args->target);
            break;
        case PORTSCAN:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=portscan&string="+args->target);
            break;
        case DISPOSABLE_MAIL_CHECK:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=disposable_email&string="+args->target);
            break;
        case IP_TO_WEBSITE:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=ip2websites&string="+args->target);
            break;
        case DOMAIN_INFO:
            url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=domaininfo&string="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://webresolver.nl/api.php?key="+m_key+"&json&action=dns&string="+args->target);
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == DNSRESOLVER){
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString type = record.toObject()["type"].toString();

            if(type == "MX" || type == "NS" || type == "CNAME"){
                QString hostname = record.toObject()["server"].toString();
                QString address = record.toObject()["ip"].toString();
                emit subdomainIp(hostname, address);
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == DNSRESOLVER){
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString address = record.toObject()["ip"].toString();
            emit ip(address);
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == DNSRESOLVER){
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString type = record.toObject()["type"].toString();

            if(type == "MX"){
                QString hostname = record.toObject()["server"].toString();
                emit MX(hostname);
                log.resultsCount++;
            }
            if(type == "NS"){
                QString hostname = record.toObject()["server"].toString();
                emit NS(hostname);
                log.resultsCount++;
            }
            if(type == "CNAME"){
                QString hostname = record.toObject()["server"].toString();
                emit CNAME(hostname);
                log.resultsCount++;
            }
            if(type == "TXT"){
                QString text = record.toObject()["text"].toString();
                emit TXT(text);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}
