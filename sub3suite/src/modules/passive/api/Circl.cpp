#include "Circl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define IP_2_ASN 0
#define PASSIVE_DNS 1
#define PASSIVE_SSL 2
#define PASSIVE_SSL_FETCH 3
#define PASSIVE_SSL_QUERY 4

/*
 * login not yet implemented
 */
Circl::Circl(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_CIRCL;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedIp);
    if(args.outputSSLCert)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedSSLCert);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedAsn);

    /* get login credentials... */
}
Circl::~Circl(){
    delete manager;
}

void Circl::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch(args.rawOption){
        case IP_2_ASN:
            url.setUrl("https://bgpranking-ng.circl.lu/ipasn_history/asn_meta");
            break;
        case PASSIVE_DNS:
            url.setUrl("https://www.circl.lu/pdns/query/"+target);
            break;
        case PASSIVE_SSL:
            url.setUrl("https://www.circl.lu/v2pssl/query/"+target);
            break;
        case PASSIVE_SSL_FETCH:
            url.setUrl("https://www.circl.lu/v2pssl/cfetch/"+target);
            break;
        case PASSIVE_SSL_QUERY:
            url.setUrl("https://www.circl.lu/v2pssl/cquery/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputIp || args.outputSubdomain){
            url.setUrl("https://www.circl.lu/pdns/query/"+target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputIp){
        if(args.outputIp || args.outputSubdomain){
            url.setUrl("https://www.circl.lu/pdns/query/"+target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputAsn){
            url.setUrl("https://bgpranking-ng.circl.lu/ipasn_history/asn_meta");
            request.setAttribute(QNetworkRequest::User, IP_2_ASN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputSSLCert){
            url.setUrl("https://www.circl.lu/pdns/query/"+target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_SSL);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputSSLCert){
        if(args.outputIp){
            url.setUrl("https://www.circl.lu/v2pssl/cquery/"+target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_SSL_QUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Circl::replyFinishedSSLCert(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = document.object();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_SSL:
        foreach(const QString &key, jsonObject.keys()){
            QJsonArray certificates = jsonObject[key].toObject()["certificates"].toArray();
            foreach(const QJsonValue &value, certificates)
            {
                emit resultSSL(value.toString());
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}

void Circl::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jsonArray = document.array();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_DNS:
        foreach(const QJsonValue &value, jsonArray){
            QString rrtype = value.toObject()["rrtype"].toString();
            QString rdata = value.toObject()["rdata"].toString();

            if(rrtype == "NS"){
                emit resultNS(rdata);
                log.resultsCount++;
            }
            if(rrtype == "MX"){
                emit resultMX(rdata);
                log.resultsCount++;
            }
            if(rrtype == "CNAME"){
                emit resultCNAME(rdata);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void Circl::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jsonArray = document.array();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_DNS:
        foreach(const QJsonValue &value, jsonArray){
            QString rrtype = value.toObject()["rrtype"].toString();
            QString rdata = value.toObject()["rdata"].toString();

            if(rrtype == "A"){
                emit resultA(rdata);
                log.resultsCount++;
            }
            if(rrtype == "AAAA"){
                emit resultAAAA(rdata);
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}

void Circl::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = document.object();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_2_ASN:
        emit resultASN(QString::number(jsonObject["asn"].toInt()), "");
        log.resultsCount++;
        break;
    }

    end(reply);
}
