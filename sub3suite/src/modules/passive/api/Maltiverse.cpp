#include "Maltiverse.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define HOSTNAME 0
#define IPV4 1
#define URL 2

/*
 * 100 requests per day
 */
Maltiverse::Maltiverse(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_MALTIVERSE;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Maltiverse::replyFinishedRawJson);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Maltiverse::replyFinishedEmail);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Maltiverse::replyFinishedAsn);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Maltiverse::replyFinishedIp);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Maltiverse::replyFinishedCidr);
}
Maltiverse::~Maltiverse(){
    delete manager;
}

void Maltiverse::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case HOSTNAME:
            url.setUrl("https://api.maltiverse.com/hostname/"+target);
            break;
        case IPV4:
            url.setUrl("https://api.maltiverse.com/ip/"+target);
            break;
        case URL:
            url.setUrl("https://api.maltiverse.com/url/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_IP || args.output_ASN){
            url.setUrl("https://api.maltiverse.com/hostname/"+target);
            request.setAttribute(QNetworkRequest::User, HOSTNAME);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_IP){
        if(args.output_ASN || args.output_CIDR || args.output_Email){
            url.setUrl("https://api.maltiverse.com/ip/"+target);
            request.setAttribute(QNetworkRequest::User, IPV4);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void Maltiverse::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOSTNAME:
    case IPV4:
        QString asn_name = mainObj["asn_name"].toString();
        if(!asn_name.isEmpty()){
            QString asnValue = asn_name.split(" ").at(0);
            QString asnName = asn_name.remove(asnValue+" ");

            emit resultASN(asnValue, asnName);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Maltiverse::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOSTNAME:
        foreach(const QJsonValue &value, mainObj["resolved_ip"].toArray()){
            QString address = value.toObject()["ip_addr"].toString();
            emit resultIP(address);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Maltiverse::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray emails = document.object()["email"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IPV4:
        foreach(const QJsonValue &value, emails){
            emit resultEmail(value.toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Maltiverse::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray cidr = document.object()["cidr"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IPV4:
        foreach(const QJsonValue &value, cidr){
            emit resultCIDR(value.toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}
