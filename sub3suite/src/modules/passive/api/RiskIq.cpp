#include "RiskIq.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define CERT_HOST 0
#define CERT_NAME 1
#define CERT_SERIAL 2
#define CERT_SHA1 3
#define HOSTS 4
#define PDNS_IP 5
#define PDNS_NAME 6
#define PDNS_RAW 7
#define WHOIS_ADDRESS 8
#define WHOIS_DOMAIN 9
#define WHOIS_EMAIL 10
#define WHOIS_NAME 11
#define WHOIS_NAMESERVER 12
#define WHOIS_ORG 13
#define WHOIS_PHONE 14

/*
 * Error in Basic Authentication
 */
RiskIq::RiskIq(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_RISKIQ;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RiskIq::replyFinishedRawJson);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RiskIq::replyFinishedIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RiskIq::replyFinishedSubdomain);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RiskIq::replyFinishedSubdomainIp);
    if(args.outputSSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RiskIq::replyFinishedSSL);

    /* getting api-key */
    m_key = APIKEY.value("riskiq_key").toString();
    m_secret = APIKEY.value("riskiq_secret").toString();
}
RiskIq::~RiskIq(){
    delete manager;
}

void RiskIq::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    QString concatenated = m_key+":"+m_secret;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", "Basic "+data);

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case CERT_HOST:
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/hos?host="+target);
            break;
        case CERT_NAME:
            url.setUrl("https://api.riskiq.net /v1/ssl/cert/name?name="+target);
            break;
        case CERT_SERIAL:
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/serial?serial="+target);
            break;
        case CERT_SHA1:
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/sha1?sha1="+target);
            break;
        case PDNS_IP:
            url.setUrl("https://api.riskiq.net/v0/pdns/data/ip?ip="+target);
            break;
        case PDNS_NAME:
            url.setUrl("https://api.riskiq.net/v0/pdns/data/name?name="+target);
            break;
        case PDNS_RAW:
            url.setUrl("https://api.riskiq.net/v0/pdns/data/raw?hex="+target);
            break;
        case WHOIS_ADDRESS:
            url.setUrl("https://api.riskiq.net/v0/whois/address?address="+target);
            break;
        case WHOIS_DOMAIN:
            url.setUrl("https://api.riskiq.net/v0/whois/domain?domain="+target);
            break;
        case WHOIS_EMAIL:
            url.setUrl("https://api.riskiq.net/v0/whois/email?email="+target);
            break;
        case WHOIS_NAME:
            url.setUrl("https://api.riskiq.net/v0/whois/name?name="+target);
            break;
        case WHOIS_NAMESERVER:
            url.setUrl("https://api.riskiq.net/v0/whois/nameserver?nameserver="+target);
            break;
        case WHOIS_ORG:
            url.setUrl("https://api.riskiq.net/v0/whois/org?org="+target);
            break;
        case WHOIS_PHONE:
            url.setUrl("https://api.riskiq.net/v0/whois/phone?phone="+target);
            break;
        case HOSTS:
            url.setUrl("https://api.riskiq.net/v1/ssl/host?certSha1="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        if(args.outputIp || args.outputSubdomain || args.outputSubdomainIp){
            url.setUrl("https://api.riskiq.net/v0/pdns/data/ip?ip="+target);
            request.setAttribute(QNetworkRequest::User, PDNS_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.outputSSL){
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/hos?host="+target);
            request.setAttribute(QNetworkRequest::User, CERT_HOST);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputDomain){
        if(args.outputIp || args.outputSubdomain || args.outputSubdomainIp){
            url.setUrl("https://api.riskiq.net/v0/pdns/data/name?name="+target);
            request.setAttribute(QNetworkRequest::User, PDNS_NAME);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.outputSSL){
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/hos?host="+target);
            request.setAttribute(QNetworkRequest::User, CERT_HOST);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputSSL){
        if(args.outputIp || args.outputSSL || args.outputSubdomain){
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/sha1?sha1="+target);
            request.setAttribute(QNetworkRequest::User, CERT_SHA1);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void RiskIq::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray records = document.object()["records"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PDNS_IP:
    case PDNS_NAME:
        foreach(const QJsonValue &record, records){
            QString rrtype = record.toObject()["rrtype"].toString();
            if(rrtype == "A" || rrtype == "AAAA"){
                QJsonArray data = record.toObject()["data"].toArray();
                QString name = record.toObject()["name"].toString();
                foreach(const QJsonValue &value, data){
                    emit resultSubdomainIp(name, value.toString());
                    log.resultsCount++;
                }
            }
        }
    }

    end(reply);
}

void RiskIq::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PDNS_IP:
    case PDNS_NAME:
    {
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString rrtype = record.toObject()["rrtype"].toString();
            if(rrtype == "A" || rrtype == "AAAA"){
                QString name = record.toObject()["name"].toString();
                emit resultSubdomain(name);
                log.resultsCount++;
            }
            if(rrtype == "NS"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit resultNS(value.toString());
                    log.resultsCount++;
                }
            }
            if(rrtype == "MX"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit resultMX(value.toString());
                    log.resultsCount++;
                }
            }
            if(rrtype == "CNAME"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit resultCNAME(value.toString());
                    log.resultsCount++;
                }
            }
        }
    }
        break;

    case CERT_HOST:
    case CERT_SHA1:
        QJsonArray content = document.object()["content"].toArray();
        foreach(const QJsonValue &record, content){
            QJsonObject cert = record.toObject()["cert"].toObject();
            foreach(const QJsonValue &altNames, cert["issuerAlternativeNames"].toArray()){
                emit resultSubdomain(altNames.toObject()["name"].toString());
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}

void RiskIq::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PDNS_IP:
    case PDNS_NAME:
    {
        QJsonArray records = document.object()["records"].toArray();
        foreach(const QJsonValue &record, records){
            QString rrtype = record.toObject()["rrtype"].toString();
            if(rrtype == "A"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit resultA(value.toString());
                    log.resultsCount++;
                }
            }
            if(rrtype == "AAAA"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit resultAAAA(value.toString());
                    log.resultsCount++;
                }
            }
        }
    }
        break;

    case CERT_SHA1:
        QJsonArray content = document.object()["content"].toArray();
        foreach(const QJsonValue &record, content){
            QJsonObject host = record.toObject()["host"].toObject();
            QString hostValue = host["host"].toString();
            emit resultIP(hostValue);
            log.resultsCount++;
        }
        break;
    }

    end(reply);
}

void RiskIq::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray content = document.object()["content"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case CERT_HOST:
    case CERT_SHA1:
        foreach(const QJsonValue &record, content){
            QJsonObject cert = record.toObject()["cert"].toObject();
            QString sha1 = cert["sha1"].toString();
            emit resultSSL(sha1);
            log.resultsCount++;
        }
    }

    end(reply);
}
