#include "VirusTotal.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DOMAINS 0
#define IP_ADDRESSES 1
#define RESOLUTIONS 2
#define URLS 3

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
    ///
    /// obtain apikey...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("virustotalapi").toString();
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
        case URLS:
            url.setUrl("https://www.virustotal.com/api/v3/urls/"+args->target);
            break;
        case DOMAINS:
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target);
            break;
        case RESOLUTIONS:
            url.setUrl("https://www.virustotal.com/api/v3/resolutions/"+args->target);
            break;
        case IP_ADDRESSES:
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target);
            break;
        }
        request.setRawHeader("x-apikey", m_key.toUtf8());
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://www.virustotal.com/vtapi/v2/domain/report?apikey="+m_key+"&domain="+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void VirusTotal::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray subdomains = document.object()["subdomains"].toArray();

    foreach(const QJsonValue &value, subdomains){
        emit subdomain(value.toString());
        log.resultsCount++;
    }
    end(reply);
}

void VirusTotal::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray resolutions = document.object()["resolutions"].toArray();

    foreach(const QJsonValue &value, resolutions){
        emit ip(value.toObject()["ip_address"].toString());
        log.resultsCount++;
    }
    end(reply);
}

void VirusTotal::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray urls = document.object()["detected_urls"].toArray();

    foreach(const QJsonValue &value, urls){
        emit url(value.toObject()["url"].toString());
        log.resultsCount++;
    }
    end(reply);
}
