#include "ZoomEye.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define HOST_ASN 0
#define HOST_CIDR 1
#define HOST_HOSTNAME 2
#define HOST_IP 3
#define USER_INFO 4

/*
 * 10K queries for free...
 * also has many query types...
 */
ZoomEye::ZoomEye(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "ZoomEye";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &ZoomEye::replyFinishedRawJson);
    if(args->outputSubdomainIp)
        connect(manager, &NetworkAccessManager::finished, this, &ZoomEye::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &ZoomEye::replyFinishedSubdomain);
    if(args->outputAsn)
        connect(manager, &NetworkAccessManager::finished, this, &ZoomEye::replyFinishedAsn);
    if(args->outputIp)
        connect(manager, &NetworkAccessManager::finished, this, &ZoomEye::replyFinishedIp);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("zoomeye").toString();
    Config::generalConfig().endGroup();
}
ZoomEye::~ZoomEye(){
    delete manager;
}

void ZoomEye::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("API-KEY", m_key.toUtf8());

    QUrl url;
    if(args->outputRaw){
        switch(args->rawOption){
        case HOST_ASN:
            url.setUrl("https://api.zoomeye.org/host/search?query=asn:"+args->target);
            break;
        case HOST_CIDR:
            url.setUrl("https://api.zoomeye.org/host/search?query=cidr:"+args->target);
            break;
        case HOST_HOSTNAME:
            url.setUrl("https://api.zoomeye.org/host/search?query=hostname:*."+args->target);
            break;
        case HOST_IP:
            url.setUrl("https://api.zoomeye.org/host/search?query=ip:"+args->target);
            break;
        case USER_INFO:
            url.setUrl("https://api.zoomeye.org/resources-info");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://api.zoomeye.org/host/search?query=hostname:*."+args->target);
        request.setAttribute(QNetworkRequest::User, HOST_HOSTNAME);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputIp){
        url.setUrl("https://api.zoomeye.org/host/search?query=ip:"+args->target);
        request.setAttribute(QNetworkRequest::User, HOST_IP);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputAsn){
        url.setUrl("https://api.zoomeye.org/host/search?query=asn:"+args->target);
        request.setAttribute(QNetworkRequest::User, HOST_ASN);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void ZoomEye::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    if(requestType == HOST_HOSTNAME || requestType == HOST_IP){
        foreach(const QJsonValue &value, matches){
            QString hostname = value.toObject()["rdns"].toString();
            QString address = value.toObject()["ip"].toString();
            emit subdomainIp(hostname, address);
            log.resultsCount++;
        }
    }

    end(reply);
}

void ZoomEye::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    if(requestType == HOST_IP || requestType == HOST_ASN || requestType == HOST_HOSTNAME){
        foreach(const QJsonValue &value, matches){
            emit ip(value.toObject()["ip"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void ZoomEye::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    if(requestType == HOST_IP || requestType == HOST_ASN || requestType == HOST_HOSTNAME){
        foreach(const QJsonValue &value, matches){
            QString ASN = value.toObject()["geoinfo"].toObject()["asn"].toString();
            QString org = value.toObject()["geoinfo"].toObject()["organization"].toString();
            emit asn(ASN, org);
            log.resultsCount++;
        }
    }

    end(reply);
}

void ZoomEye::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    if(requestType == HOST_IP || requestType == HOST_HOSTNAME){
        foreach(const QJsonValue &value, matches){
            emit subdomain(value.toObject()["rdns"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
