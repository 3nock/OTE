#include "FullHunt.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DOMAIN_DETAILS 0
#define DOMAIN_SUBDOMAINS 1
#define HOST_DETAILS 2

/*
 * https://api-docs.fullhunt.io/#rate-limiting
 * some error with domain queries...
 */
FullHunt::FullHunt(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_FULLHUNT;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &FullHunt::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &FullHunt::replyFinishedSubdomain);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &FullHunt::replyFinishedAsn);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &FullHunt::replyFinishedIp);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_FULLHUNT).toString();
    
}
FullHunt::~FullHunt(){
    delete manager;
}

void FullHunt::start(){
    QNetworkRequest request;
    request.setRawHeader("X-API-KEY", m_key.toUtf8());

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case DOMAIN_DETAILS:
            url.setUrl("https://fullhunt.io/api/v1/domain/"+target+"/details");
            break;
        case DOMAIN_SUBDOMAINS:
            url.setUrl("https://fullhunt.io/api/v1/domain/"+target+"/subdomains");
            break;
        case HOST_DETAILS:
            url.setUrl("https://fullhunt.io/api/v1/host/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.input_Domain){
        if(args.output_Hostname){
            QUrl url("https://jldc.me/FullHunt/subdomains/"+target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.output_ASN){
            url.setUrl("https://fullhunt.io/api/v1/domain/"+target+"/details");
            request.setAttribute(QNetworkRequest::User, DOMAIN_DETAILS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void FullHunt::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case DOMAIN_SUBDOMAINS:
        foreach(const QJsonValue &value, document.object()["subdomains"].toArray()){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
        break;
    }

    end(reply);
}

void FullHunt::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case DOMAIN_DETAILS:
        foreach(const QJsonValue &value, document.object()["hosts"].toArray()){
            QJsonObject ip_metadata = value.toObject()["ip_metadata"].toObject();
            int asn = ip_metadata["asn"].toInt();
            QString isp = ip_metadata["isp"].toString();
            emit resultASN(QString::number(asn), isp);
            log.resultsCount++;
        }
        break;
    }

    end(reply);
}

void FullHunt::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case DOMAIN_DETAILS:
        foreach(const QJsonValue &value, document.object()["hosts"].toArray()){
            QJsonObject dns = value.toObject()["dns"].toObject();
            foreach(const QJsonValue &a, dns["a"].toArray()){
                emit resultA(a.toString());
                log.resultsCount++;
            }
            foreach(const QJsonValue &aaaa, dns["aaaa"].toArray()){
                emit resultAAAA(aaaa.toString());
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}

