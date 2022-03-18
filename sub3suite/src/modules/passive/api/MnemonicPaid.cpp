#include "MnemonicPaid.h"
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


MnemonicPaid::MnemonicPaid(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_MNEMONIC;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &MnemonicPaid::replyFinishedRawJson);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &MnemonicPaid::replyFinishedIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &MnemonicPaid::replyFinishedSubdomain);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_MNEMONIC).toString();
    
}
MnemonicPaid::~MnemonicPaid(){
    delete manager;
}

void MnemonicPaid::start(){
    QNetworkRequest request;
    request.setRawHeader("Argus-API-Key", m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
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

    if(args.inputIp){
        if(args.outputSubdomain || args.outputIp){
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?limit=999");
            request.setAttribute(QNetworkRequest::User, IP_ANY_RECORD);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputDomain){
        if(args.outputSubdomain || args.outputIp){
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+target+"?limit=999");
            request.setAttribute(QNetworkRequest::User, IP_ANY_RECORD);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void MnemonicPaid::replyFinishedIp(QNetworkReply *reply){
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

void MnemonicPaid::replyFinishedSubdomain(QNetworkReply *reply){
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
