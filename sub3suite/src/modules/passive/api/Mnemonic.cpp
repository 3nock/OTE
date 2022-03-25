#include "Mnemonic.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define PDNS_A 0
#define PDNS_AAAA 1
#define PDNS_CNAME 2
#define PDNS_MX 3
#define PDNS_NS 4
#define IP_ANY_RECORD 5


Mnemonic::Mnemonic(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_MNEMONIC;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Mnemonic::replyFinishedRawJson);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Mnemonic::replyFinishedIp);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Mnemonic::replyFinishedSubdomain);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_MNEMONIC).toString();
    
}
Mnemonic::~Mnemonic(){
    delete manager;
}

void Mnemonic::start(){
    QNetworkRequest request;
    request.setRawHeader("Argus-API-Key", m_key.toUtf8());

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case IP_ANY_RECORD:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?limit=999");
            break;
        case PDNS_A:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?rrType=A&limit=999");
            break;
        case PDNS_AAAA:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?rrType=AAAA&limit=999");
            break;
        case PDNS_CNAME:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?rrType=CNAME&limit=999");
            break;
        case PDNS_MX:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?rrType=MX&limit=999");
            break;
        case PDNS_NS:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?rrType=NS&limit=999");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args.input_IP){
        if(args.output_Hostname || args.output_IP){
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?limit=999");
            request.setAttribute(QNetworkRequest::User, IP_ANY_RECORD);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_Domain){
        if(args.output_Hostname || args.output_IP){
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?limit=999");
            request.setAttribute(QNetworkRequest::User, IP_ANY_RECORD);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Mnemonic::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_ANY_RECORD:
        foreach(const QJsonValue &value, data){
            QJsonObject dataObj = value.toObject();
            if(dataObj["rrtype"].toString() == "a"){
                emit resultA(dataObj["answer"].toString());
                log.resultsCount++;
            }
            if(dataObj["rrtype"].toString() == "aaaa"){
                emit resultAAAA(dataObj["answer"].toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void Mnemonic::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray data = document.object()["data"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_ANY_RECORD:
        foreach(const QJsonValue &value, data){
            emit resultSubdomain(value.toObject()["query"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
