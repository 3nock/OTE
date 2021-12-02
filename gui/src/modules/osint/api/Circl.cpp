#include "Circl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define IP_2_ASN 0
#define PASSIVE_DNS 1
#define PASSIVE_SSL 2
#define PASSIVE_SSL_FETCH 3
#define PASSIVE_SSL_QUERY 4

Circl::Circl(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Circl";

    if(args->outputRaw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Circl::replyFinishedRawJson);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Circl::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Circl::replyFinishedIp);
    if(args->outputSSLCert)
        connect(manager, &MyNetworkAccessManager::finished, this, &Circl::replyFinishedSSLCert);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &Circl::replyFinishedAsn);
    ///
    /// get login credentials...
    ///
}
Circl::~Circl(){
    delete manager;
}

void Circl::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch(args->rawOption){
        case IP_2_ASN:
            url.setUrl("https://bgpranking-ng.circl.lu/ipasn_history/asn_meta");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        case PASSIVE_DNS:
            url.setUrl("https://www.circl.lu/pdns/query/"+args->target);
            break;
        case PASSIVE_SSL:
            url.setUrl("https://www.circl.lu/v2pssl/query/"+args->target);
            break;
        case PASSIVE_SSL_FETCH:
            url.setUrl("https://www.circl.lu/v2pssl/cfetch/"+args->target);
            break;
        case PASSIVE_SSL_QUERY:
            url.setUrl("https://www.circl.lu/v2pssl/cquery/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputIp || args->outputSubdomain){
            url.setUrl("https://www.circl.lu/pdns/query/"+args->target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args->inputIp){
        if(args->outputIp || args->outputSubdomain){
            url.setUrl("https://www.circl.lu/pdns/query/"+args->target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args->outputAsn){
            url.setUrl("https://bgpranking-ng.circl.lu/ipasn_history/asn_meta");
            request.setAttribute(QNetworkRequest::User, IP_2_ASN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args->outputSSLCert){
            url.setUrl("https://www.circl.lu/pdns/query/"+args->target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_SSL);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args->inputSSLCert){
        /* returns the certificate
        if(args->outputSSLCert){
            url.setUrl("https://www.circl.lu/pdns/query/"+args->target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_SSL_FETCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        */
        if(args->outputIp){
            url.setUrl("https://www.circl.lu/v2pssl/cquery/"+args->target);
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = document.object();

    if(requestType == PASSIVE_SSL){
        QStringList keys = jsonObject.keys();
        foreach(const QString &key, keys){
            QJsonArray certificates = jsonObject[key].toObject()["certificates"].toArray();
            foreach(const QJsonValue &value, certificates){
                emit sslCert(value.toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Circl::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jsonArray = document.array();

    if(requestType == PASSIVE_DNS){
        foreach(const QJsonValue &value, jsonArray){
            QString rrtype = value.toObject()["rrtype"].toString();
            QString rdata = value.toObject()["rdata"].toString();
            if(rrtype == "NS"){
                emit NS(rdata);
                log.resultsCount++;
            }
            if(rrtype == "MX"){
                emit MX(rdata);
                log.resultsCount++;
            }
            if(rrtype == "CNAME"){
                emit CNAME(rdata);
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jsonArray = document.array();

    if(requestType == PASSIVE_DNS){
        foreach(const QJsonValue &value, jsonArray){
            QString rrtype = value.toObject()["rrtype"].toString();
            QString rdata = value.toObject()["rdata"].toString();
            if(rrtype == "A"){
                emit ipA(rdata);
                log.resultsCount++;
            }
            if(rrtype == "AAAA"){
                emit ipAAAA(rdata);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Circl::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = document.object();

    if(requestType == IP_2_ASN){
        QString value = QString::number(jsonObject["asn"].toInt());
        emit asn(value, "");
        log.resultsCount++;
    }
    end(reply);
}
