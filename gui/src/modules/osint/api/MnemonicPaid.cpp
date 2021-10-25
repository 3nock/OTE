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

/*
 * access passive-dns, 1000 queries per day for public usage
 * you can query for any dns record type
 */
MnemonicPaid::MnemonicPaid(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Mnemonic";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &MnemonicPaid::replyFinishedRaw);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &MnemonicPaid::replyFinishedIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &MnemonicPaid::replyFinishedRaw);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("mnemonic").toString();
    Config::generalConfig().endGroup();
}
MnemonicPaid::~MnemonicPaid(){
    delete manager;
}

void MnemonicPaid::start(){
    QNetworkRequest request;
    request.setRawHeader("Argus-API-Key", m_key.toUtf8());

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case IP_ANY_RECORD:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?limit=999");
            break;
        case PDNS_A:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=A&limit=999");
            break;
        case PDNS_AAAA:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=AAAA&limit=999");
            break;
        case PDNS_CNAME:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=CNAME&limit=999");
            break;
        case PDNS_MX:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=MX&limit=999");
            break;
        case PDNS_NS:
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=NS&limit=999");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputIp){
        url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?limit=999");
        request.setAttribute(QNetworkRequest::User, IP_ANY_RECORD);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputDomain){
        if(args->outputIp){
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=A&limit=999");
            request.setAttribute(QNetworkRequest::User, PDNS_A);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=AAAA&limit=999");
            request.setAttribute(QNetworkRequest::User, PDNS_AAAA);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputSubdomain){
            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=CNAME&limit=999");
            request.setAttribute(QNetworkRequest::User, PDNS_CNAME);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=MX&limit=999");
            request.setAttribute(QNetworkRequest::User, PDNS_MX);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.mnemonic.no/pdns/v3/"+args->target+"?rrType=NS&limit=999");
            request.setAttribute(QNetworkRequest::User, PDNS_NS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void MnemonicPaid::replyFinishedIp(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray data = document.object()["data"].toArray();
        //int responseCode = document.object()["responseCode"].toInt();

        if(requestType == IP_ANY_RECORD){
            foreach(const QJsonValue &value, data){
                QJsonObject dataObj = value.toObject();
                if(dataObj["rrtype"].toString() == "a"){
                    emit ipA(dataObj["answer"].toString());
                    log.resultsCount++;
                }
                if(dataObj["rrtype"].toString() == "aaaa"){
                    emit ipAAAA(dataObj["answer"].toString());
                    log.resultsCount++;
                }
            }
        }

        if(requestType == PDNS_A){
            foreach(const QJsonValue &value, data){
                QJsonObject dataObj = value.toObject();
                if(dataObj["rrtype"].toString() == "a"){
                    emit ipA(dataObj["answer"].toString());
                    log.resultsCount++;
                }
            }
        }

        if(requestType == PDNS_AAAA){
            foreach(const QJsonValue &value, data){
                QJsonObject dataObj = value.toObject();
                if(dataObj["rrtype"].toString() == "aaaa"){
                    emit ipAAAA(dataObj["answer"].toString());
                    log.resultsCount++;
                }
            }
        }
    }
    end(reply);
}

void MnemonicPaid::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray data = document.object()["data"].toArray();
        //int responseCode = document.object()["responseCode"].toInt();

        if(requestType == IP_ANY_RECORD){
            foreach(const QJsonValue &value, data){
                QJsonObject dataObj = value.toObject();
                if(dataObj["rrtype"].toString() == "cname"){
                    emit CNAME(dataObj["answer"].toString());
                    log.resultsCount++;
                }
                if(dataObj["rrtype"].toString() == "mx"){
                    emit MX(dataObj["answer"].toString());
                    log.resultsCount++;
                }
                if(dataObj["rrtype"].toString() == "ns"){
                    emit NS(dataObj["answer"].toString());
                    log.resultsCount++;
                }
            }
        }

        if(requestType == PDNS_CNAME){
            foreach(const QJsonValue &value, data){
                QJsonObject dataObj = value.toObject();
                if(dataObj["rrtype"].toString() == "cname"){
                    emit CNAME(dataObj["answer"].toString());
                    log.resultsCount++;
                }
            }
        }

        if(requestType == PDNS_MX){
            foreach(const QJsonValue &value, data){
                QJsonObject dataObj = value.toObject();
                if(dataObj["rrtype"].toString() == "mx"){
                    emit MX(dataObj["answer"].toString());
                    log.resultsCount++;
                }
            }
        }

        if(requestType == PDNS_NS){
            foreach(const QJsonValue &value, data){
                QJsonObject dataObj = value.toObject();
                if(dataObj["rrtype"].toString() == "ns"){
                    emit NS(dataObj["answer"].toString());
                    log.resultsCount++;
                }
            }
        }
    }
    end(reply);
}
