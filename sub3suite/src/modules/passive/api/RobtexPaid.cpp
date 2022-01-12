#include "RobtexPaid.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASQUERY 0
#define IPQUERY 1
#define PDNS_FORWARD 2
#define PDNS_REVERSE 3


RobtexPaid::RobtexPaid(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "RobtexPaid";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexPaid::replyFinishedRawJson);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexPaid::replyFinishedIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexPaid::replyFinishedSubdomain);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexPaid::replyFinishedSubdomainIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &RobtexPaid::replyFinishedAsn);
    ///
    /// getting api key...
    ///
    
    m_key = APIKEY.value("robtex").toString();
    
}
RobtexPaid::~RobtexPaid(){
    delete manager;
}

void RobtexPaid::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ASQUERY:
            url.setUrl("https://proapi.robtex.com/asquery/"+target+"?key="+m_key);
            break;
        case IPQUERY:
            url.setUrl("https://proapi.robtex.com/ipquery/"+target+"?key="+m_key);
            break;
        case PDNS_FORWARD:
            url.setUrl("https://proapi.robtex.com/pdns/forward/"+target+"?key="+m_key);
            break;
        case PDNS_REVERSE:
            url.setUrl("https://proapi.robtex.com/pdns/reverse/"+target+"?key="+m_key);
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        if(args.outputIp || args.outputAsn){
            url.setUrl("https://proapi.robtex.com/ipquery/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, IPQUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputIp || args.outputSubdomain || args.outputSubdomainIp){
            url.setUrl("https://proapi.robtex.com/pdns/reverse/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, PDNS_REVERSE);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args.inputDomain){
        url.setUrl("https://proapi.robtex.com/pdns/forward/"+target+"?key="+m_key);
        request.setAttribute(QNetworkRequest::User, PDNS_FORWARD);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputAsn){
        url.setUrl("https://proapi.robtex.com/asquery/"+target+"?key="+m_key);
        request.setAttribute(QNetworkRequest::User, ASQUERY);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void RobtexPaid::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == PDNS_FORWARD || QUERY_TYPE == PDNS_REVERSE){
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

    if(QUERY_TYPE == IPQUERY){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

        QJsonArray act = document.object()["act"].toArray();
        foreach(const QJsonValue &value, act){
            QString host = value.toObject()["o"].toString();
            emit resultSubdomain(host);
            log.resultsCount++;
        }

        QJsonArray pas = document.object()["pas"].toArray();
        foreach(const QJsonValue &value, pas){
            QString host = value.toObject()["o"].toString();
            emit resultSubdomain(host);
            log.resultsCount++;
        }
    }
    end(reply);
}

void RobtexPaid::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == PDNS_FORWARD || QUERY_TYPE == PDNS_REVERSE){
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

    if(QUERY_TYPE == ASQUERY){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray nets = document.object()["nets"].toArray();
        foreach(const QJsonValue &value, nets){
            QString address = value.toObject()["n"].toString();
            emit resultIp(address);
            //emit resultCidr(address); since the return address is a ip/cdir..
            log.resultsCount++;
        }
    }
    end(reply);
}

void RobtexPaid::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == IPQUERY){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject mainObj = document.object();
        QString asnValue = mainObj["as"].toString();
        QString asnName = mainObj["asname"].toString();

        emit resultASN(asnValue, asnName);
        log.resultsCount++;
    }
    end(reply);
}

void RobtexPaid::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == PDNS_FORWARD || QUERY_TYPE == PDNS_REVERSE){
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
