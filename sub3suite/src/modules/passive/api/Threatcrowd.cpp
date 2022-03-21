#include "Threatcrowd.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define ANTIVIRUS 0
#define DOMAINS 1
#define EMAIL 2
#define FILE 3
#define IP 4

/*  limit all requests to no more than one request every ten seconds */
Threatcrowd::Threatcrowd(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "ThreatCrowd";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatcrowd::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatcrowd::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatcrowd::replyFinishedIp);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatcrowd::replyFinishedEmail);
}
Threatcrowd::~Threatcrowd(){
    delete manager;
}

void Threatcrowd::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case EMAIL:
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/email/report/?email="+target);
            break;
        case DOMAINS:
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/domain/report/?domain="+target);
            break;
        case IP:
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/ip/report/?ip="+target);
            break;
        case ANTIVIRUS:
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/antivirus/report/?antivirus="+target);
            break;
        case FILE:
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/file/report/?resource="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        url.setUrl("https://www.threatcrowd.org/searchApi/v2/domain/report/?domain="+target);
        request.setAttribute(QNetworkRequest::User, DOMAINS);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputEmail){
        url.setUrl("https://www.threatcrowd.org/searchApi/v2/email/report/?email="+target);
        request.setAttribute(QNetworkRequest::User, EMAIL);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        url.setUrl("https://www.threatcrowd.org/searchApi/v2/ip/report/?ip="+target);
        request.setAttribute(QNetworkRequest::User, IP);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Threatcrowd::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    if(QUERY_TYPE == DOMAINS){
        QJsonArray subdomains = mainObj["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomains){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
    }

    if(QUERY_TYPE == EMAIL){
        if(mainObj["response_code"].toString() == "1"){
            QJsonArray domains = mainObj["domains"].toArray();
            foreach(const QJsonValue &value, domains){
                emit resultSubdomain(value.toString());
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == IP){
        if(mainObj["response_code"].toString() == "1"){
            QJsonArray resolutions = mainObj["resolutions"].toArray();
            foreach(const QJsonValue &value, resolutions){
                emit resultSubdomain(value.toObject()["domain"].toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Threatcrowd::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    if(QUERY_TYPE == DOMAINS){
        if(mainObj["response_code"].toString() == "1"){
            QJsonArray resolutions = mainObj["resolutions"].toArray();
            foreach(const QJsonValue &value, resolutions){
                emit resultIP(value["ip_address"].toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Threatcrowd::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    if(QUERY_TYPE == DOMAINS){
        if(mainObj["response_code"].toString() == "1"){
            QJsonArray emails = mainObj["emails"].toArray();
            foreach(const QJsonValue &value, emails){
                QString EmailAddress = value.toString();
                emit resultEmail(EmailAddress);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}
