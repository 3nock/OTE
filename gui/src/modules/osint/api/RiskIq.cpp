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
 * returns hosts using cert-id HOSTS
 */
RiskIq::RiskIq(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "RiskIq";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &RiskIq::replyFinishedRaw);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &RiskIq::replyFinishedIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &RiskIq::replyFinishedSubdomain);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &RiskIq::replyFinishedSubdomainIp);
    if(args->outputCertFingerprint)
        connect(manager, &MyNetworkAccessManager::finished, this, &RiskIq::replyFinishedCertFingerprint);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_name = Config::generalConfig().value("riskiq_username").toString();
    m_key = Config::generalConfig().value("riskiq_key").toString();
    Config::generalConfig().endGroup();
}
RiskIq::~RiskIq(){
    delete manager;
}

void RiskIq::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    ///
    /// HTTP Basic authentication header value: base64(username:password)
    ///
    QString concatenated = m_name+":"+m_key;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", "Basic "+data);

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case CERT_HOST:
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/hos?host="+args->target);
            break;
        case CERT_NAME:
            url.setUrl("https://api.riskiq.net /v1/ssl/cert/name?name="+args->target);
            break;
        case CERT_SERIAL:
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/serial?serial="+args->target);
            break;
        case CERT_SHA1:
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/sha1?sha1="+args->target);
            break;
        case PDNS_IP:
            url.setUrl("https://api.riskiq.net/v0/pdns/data/ip?ip="+args->target);
            break;
        case PDNS_NAME:
            url.setUrl("https://api.riskiq.net/v0/pdns/data/name?name="+args->target);
            break;
        case PDNS_RAW:
            url.setUrl("https://api.riskiq.net/v0/pdns/data/raw?hex="+args->target);
            break;
        case WHOIS_ADDRESS:
            url.setUrl("https://api.riskiq.net/v0/whois/address?address="+args->target);
            break;
        case WHOIS_DOMAIN:
            url.setUrl("https://api.riskiq.net/v0/whois/domain?domain="+args->target);
            break;
        case WHOIS_EMAIL:
            url.setUrl("https://api.riskiq.net/v0/whois/email?email="+args->target);
            break;
        case WHOIS_NAME:
            url.setUrl("https://api.riskiq.net/v0/whois/name?name="+args->target);
            break;
        case WHOIS_NAMESERVER:
            url.setUrl("https://api.riskiq.net/v0/whois/nameserver?nameserver="+args->target);
            break;
        case WHOIS_ORG:
            url.setUrl("https://api.riskiq.net/v0/whois/org?org="+args->target);
            break;
        case WHOIS_PHONE:
            url.setUrl("https://api.riskiq.net/v0/whois/phone?phone="+args->target);
            break;
        case HOSTS:
            url.setUrl("https://api.riskiq.net/v1/ssl/host?certSha1="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputIp){
        if(args->outputIp || args->outputSubdomain || args->outputSubdomainIp){
            url.setUrl("https://api.riskiq.net/v0/pdns/data/ip?ip="+args->target);
            request.setAttribute(QNetworkRequest::User, PDNS_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputCertFingerprint){
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/hos?host="+args->target);
            request.setAttribute(QNetworkRequest::User, CERT_HOST);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputDomain){
        if(args->outputIp || args->outputSubdomain || args->outputSubdomainIp){
            url.setUrl("https://api.riskiq.net/v0/pdns/data/name?name="+args->target);
            request.setAttribute(QNetworkRequest::User, PDNS_NAME);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputCertFingerprint){
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/hos?host="+args->target);
            request.setAttribute(QNetworkRequest::User, CERT_HOST);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputCertFingerprint){
        if(args->outputIp || args->outputCertFingerprint){
            url.setUrl("https://api.riskiq.net/v1/ssl/cert/sha1?sha1="+args->target);
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray records = document.object()["records"].toArray();

    if(requestType == PDNS_IP || requestType == PDNS_NAME){
        foreach(const QJsonValue &record, records){
            QString rrtype = record.toObject()["rrtype"].toString();
            if(rrtype == "A" || rrtype == "AAAA"){
                QJsonArray data = record.toObject()["data"].toArray();
                QString name = record.toObject()["name"].toString();
                foreach(const QJsonValue &value, data){
                    emit subdomainIp(name, value.toString());
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray records = document.object()["records"].toArray();

    if(requestType == PDNS_IP || requestType == PDNS_NAME){
        foreach(const QJsonValue &record, records){
            QString rrtype = record.toObject()["rrtype"].toString();
            if(rrtype == "A" || rrtype == "AAAA"){
                QString name = record.toObject()["name"].toString();
                emit subdomain(name);
                log.resultsCount++;
            }
            if(rrtype == "NS"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit NS(value.toString());
                    log.resultsCount++;
                }
            }
            if(rrtype == "MX"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit MX(value.toString());
                    log.resultsCount++;
                }
            }
            if(rrtype == "CNAME"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit CNAME(value.toString());
                    log.resultsCount++;
                }
            }
        }
    }
    end(reply);
}

void RiskIq::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();

    if(requestType == PDNS_IP || requestType == PDNS_NAME){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray records = document.object()["records"].toArray();

        foreach(const QJsonValue &record, records){
            QString rrtype = record.toObject()["rrtype"].toString();
            if(rrtype == "A"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit ipA(value.toString());
                    log.resultsCount++;
                }
            }
            if(rrtype == "AAAA"){
                QJsonArray data = record.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data){
                    emit ipAAAA(value.toString());
                    log.resultsCount++;
                }
            }
        }
    }

    if(requestType == CERT_HOST || requestType == CERT_SHA1){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray content = document.object()["content"].toArray();

        foreach(const QJsonValue &record, content){
            QJsonObject host = record.toObject()["host"].toObject();
            QString hostValue = host["host"].toString();
            emit ip(hostValue);
            log.resultsCount++;
        }
    }
    end(reply);
}

void RiskIq::replyFinishedCertFingerprint(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray content = document.object()["content"].toArray();

    if(requestType == CERT_HOST || requestType == CERT_SHA1){
        foreach(const QJsonValue &record, content){
            QJsonObject cert = record.toObject()["cert"].toObject();
            QString sha1 = cert["sha1"].toString();
            emit certFingerprint(sha1);
            log.resultsCount++;
            /* QString serialNo = cert["serialNumber"].toString(); */
        }
    }
    end(reply);
}
