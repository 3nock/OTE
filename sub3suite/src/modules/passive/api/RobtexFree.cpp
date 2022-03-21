#include "RobtexFree.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASQUERY 0
#define IPQUERY 1
#define PDNS_FORWARD 2
#define PDNS_REVERSE 3


RobtexFree::RobtexFree(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_ROBTEX;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexFree::replyFinishedRawJson);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexFree::replyFinishedIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexFree::replyFinishedSubdomain);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexFree::replyFinishedSubdomainIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexFree::replyFinishedAsn);
    if(args.outputCidr)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexFree::replyFinishedCidr);
}
RobtexFree::~RobtexFree(){
    delete manager;
}

void RobtexFree::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ASQUERY:
            url.setUrl("https://freeapi.robtex.com/asquery/"+target);
            break;
        case IPQUERY:
            url.setUrl("https://freeapi.robtex.com/ipquery/"+target);
            break;
        case PDNS_FORWARD:
            url.setUrl("https://freeapi.robtex.com/pdns/forward/"+target);
            break;
        case PDNS_REVERSE:
            url.setUrl("https://freeapi.robtex.com/pdns/reverse/"+target);
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        if(args.outputCidr || args.outputAsn){
            url.setUrl("https://freeapi.robtex.com/ipquery/"+target);
            request.setAttribute(QNetworkRequest::User, IPQUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputIp || args.outputSubdomain || args.outputSubdomainIp){
            url.setUrl("https://freeapi.robtex.com/pdns/reverse/"+target);
            request.setAttribute(QNetworkRequest::User, PDNS_REVERSE);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputDomain){
        if(args.outputSubdomainIp || args.outputSubdomain || args.outputIp){
            url.setUrl("https://freeapi.robtex.com/pdns/forward/"+target);
            request.setAttribute(QNetworkRequest::User, PDNS_FORWARD);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputAsn){
        if(args.outputCidr){
            url.setUrl("https://freeapi.robtex.com/asquery/"+target);
            request.setAttribute(QNetworkRequest::User, ASQUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void RobtexFree::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PDNS_FORWARD:
    case PDNS_REVERSE:
        QString stringReply = QString::fromUtf8(reply->readAll());
        QStringList results = stringReply.split("\n");

        foreach(const QString &result, results){
            QJsonDocument document  = QJsonDocument::fromJson(result.toUtf8());
            QString rrtype = document.object()["rrtype"].toString();
            QString rrname = document.object()["rrname"].toString();
            QString rrdata = document.object()["rrdata"].toString();
            if(rrtype == "A" || rrtype == "AAAA"){
                emit resultSubdomain(rrname);
                log.resultsCount++;
            }
            if(rrtype == "NS"){
                emit resultNS(rrdata);
                log.resultsCount++;
            }
            if(rrtype == "MX"){
                emit resultMX(rrdata);
                log.resultsCount++;
            }
            if(rrtype == "CNAME"){
                emit resultCNAME(rrdata);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void RobtexFree::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PDNS_FORWARD:
    case PDNS_REVERSE:
    {
        QString stringReply = QString::fromUtf8(reply->readAll());
        QStringList results = stringReply.split("\n");

        foreach(const QString &result, results){
            QJsonDocument document  = QJsonDocument::fromJson(result.toUtf8());
            QString rrtype = document.object()["rrtype"].toString();
            QString rrname = document.object()["rrname"].toString();
            QString rrdata = document.object()["rrdata"].toString();
            if(rrtype == "A"){
                emit resultA(rrdata);
                log.resultsCount++;
            }
            if(rrtype == "A"){
                emit resultAAAA(rrdata);
                log.resultsCount++;
            }
        }
    }
    }

    end(reply);
}

void RobtexFree::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IPQUERY:
        QJsonObject mainObj = document.object();

        emit resultASN(mainObj["as"].toString(), mainObj["asname"].toString());
        log.resultsCount++;
    }

    end(reply);
}

void RobtexFree::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IPQUERY:
    {
        emit resultCIDR(document.object()["bgproute"].toString());
        log.resultsCount++;
    }
        break;
    case ASQUERY:
        foreach(const QJsonValue &value, document.object()["nets"].toArray()){
            emit resultCIDR(value.toObject()["n"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void RobtexFree::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PDNS_FORWARD:
    case PDNS_REVERSE:
        QString stringReply = QString::fromUtf8(reply->readAll());
        QStringList results = stringReply.split("\n");

        foreach(const QString &result, results){
            QJsonDocument document  = QJsonDocument::fromJson(result.toUtf8());
            QString rrtype = document.object()["rrtype"].toString();
            QString rrname = document.object()["rrname"].toString();
            QString rrdata = document.object()["rrdata"].toString();
            if(rrtype == "A" || rrtype == "AAAA"){
                emit resultSubdomainIp(rrname, rrdata);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}
