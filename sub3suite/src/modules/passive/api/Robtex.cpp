#include "Robtex.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASQUERY 0
#define IPQUERY 1
#define PDNS_FORWARD 2
#define PDNS_REVERSE 3


Robtex::Robtex(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_ROBTEX;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Robtex::replyFinishedRawJson);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Robtex::replyFinishedIp);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Robtex::replyFinishedSubdomain);
    if(args.output_HostnameIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Robtex::replyFinishedSubdomainIp);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Robtex::replyFinishedAsn);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Robtex::replyFinishedCidr);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_ROBTEX).toString();
    
}
Robtex::~Robtex(){
    delete manager;
}

void Robtex::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
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

    if(args.input_IP){
        if(args.output_CIDR || args.output_ASN){
            url.setUrl("https://proapi.robtex.com/ipquery/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, IPQUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.output_IP || args.output_Hostname || args.output_HostnameIP){
            url.setUrl("https://proapi.robtex.com/pdns/reverse/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, PDNS_REVERSE);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_Domain){
        if(args.output_IP || args.output_Hostname || args.output_HostnameIP){
            url.setUrl("https://proapi.robtex.com/pdns/forward/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, PDNS_FORWARD);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.input_ASN){
        if(args.output_CIDR){
            url.setUrl("https://proapi.robtex.com/asquery/"+target+"?key="+m_key);
            request.setAttribute(QNetworkRequest::User, ASQUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Robtex::replyFinishedSubdomain(QNetworkReply *reply){
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

void Robtex::replyFinishedIp(QNetworkReply *reply){
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

void Robtex::replyFinishedAsn(QNetworkReply *reply){
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

void Robtex::replyFinishedCidr(QNetworkReply *reply){
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

void Robtex::replyFinishedSubdomainIp(QNetworkReply *reply){
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