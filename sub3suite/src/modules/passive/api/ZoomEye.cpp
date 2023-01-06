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
ZoomEye::ZoomEye(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_ZOOMEYE;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ZoomEye::replyFinishedRawJson);
    if(args.output_HostnameIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ZoomEye::replyFinishedSubdomainIp);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ZoomEye::replyFinishedSubdomain);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ZoomEye::replyFinishedAsn);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ZoomEye::replyFinishedIp);

    /* getting api key */
    m_key = gConfig.keys.ZoomEye;
}
ZoomEye::~ZoomEye(){
    delete manager;
}

void ZoomEye::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("API-KEY", m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case HOST_ASN:
            url.setUrl("https://api.zoomeye.org/host/search?query=asn:"+target);
            break;
        case HOST_CIDR:
            url.setUrl("https://api.zoomeye.org/host/search?query=cidr:"+target);
            break;
        case HOST_HOSTNAME:
            url.setUrl("https://api.zoomeye.org/host/search?query=hostname:*."+target);
            break;
        case HOST_IP:
            url.setUrl("https://api.zoomeye.org/host/search?query=ip:"+target);
            break;
        case USER_INFO:
            url.setUrl("https://api.zoomeye.org/resources-info");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        url.setUrl("https://api.zoomeye.org/host/search?query=hostname:*."+target);
        request.setAttribute(QNetworkRequest::User, HOST_HOSTNAME);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_IP){
        url.setUrl("https://api.zoomeye.org/host/search?query=ip:"+target);
        request.setAttribute(QNetworkRequest::User, HOST_IP);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_ASN){
        url.setUrl("https://api.zoomeye.org/host/search?query=asn:"+target);
        request.setAttribute(QNetworkRequest::User, HOST_ASN);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_CIDR){
        url.setUrl("https://api.zoomeye.org/host/search?query=cidr:"+target);
        request.setAttribute(QNetworkRequest::User, HOST_CIDR);
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

void ZoomEye::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOST_HOSTNAME:
    case HOST_IP:
        foreach(const QJsonValue &value, matches){
            QString hostname = value.toObject()["rdns"].toString();
            QString address = value.toObject()["ip"].toString();
            emit resultSubdomainIp(hostname, address);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void ZoomEye::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOST_IP:
    case HOST_ASN:
    case HOST_CIDR:
    case HOST_HOSTNAME:
        foreach(const QJsonValue &value, matches){
            emit resultIP(value.toObject()["ip"].toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void ZoomEye::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOST_IP:
    case HOST_ASN:
    case HOST_CIDR:
    case HOST_HOSTNAME:
        foreach(const QJsonValue &value, matches){
            QString ASN = value.toObject()["geoinfo"].toObject()["asn"].toString();
            QString org = value.toObject()["geoinfo"].toObject()["organization"].toString();
            emit resultASN(ASN, org);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void ZoomEye::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray matches = document.object()["matches"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOST_IP:
    case HOST_HOSTNAME:
        foreach(const QJsonValue &value, matches){
            emit resultSubdomain(value.toObject()["rdns"].toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}
