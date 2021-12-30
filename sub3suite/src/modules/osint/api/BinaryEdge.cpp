#include "BinaryEdge.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define DOMAIN_DNS 0
#define DOMAIN_ENUMERATION 1
#define DOMAIN_HYMOGLYPHS 2
#define DOMAIN_IP 3
#define DOMAIN_SEARCH 4
#define DOMAIN_SUBDOMAIN 5
#define HOST_HISTORICAL 6
#define HOST_IP 7
#define HOST_SEARCH 8
#define HOST_SEARCHSTAT 9
#define SUBSCRIPTION 10


BinaryEdge::BinaryEdge(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = OSINT_MODULE_BINARYEDGE;

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &BinaryEdge::replyFinishedRawJson);
    if(args.outputSubdomainIp)
        connect(manager, &NetworkAccessManager::finished, this, &BinaryEdge::replyFinishedSubdomainIp);
    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &BinaryEdge::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &NetworkAccessManager::finished, this, &BinaryEdge::replyFinishedIp);

    /* getting api key... */
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value(OSINT_MODULE_BINARYEDGE).toString();
    Config::generalConfig().endGroup();
}
BinaryEdge::~BinaryEdge(){
    delete manager;
}

void BinaryEdge::start(){
    QNetworkRequest request;
    request.setRawHeader("X-KEY", m_key.toUtf8());
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case DOMAIN_DNS:
            url.setUrl("https://api.binaryedge.io/v2/query/domains/dns/"+target);
            break;
        case DOMAIN_ENUMERATION:
            url.setUrl("https://api.binaryedge.io/v2/query/domains/enumeration/"+target);
            break;
        case DOMAIN_HYMOGLYPHS:
            url.setUrl("https://api.binaryedge.io/v2/query/domains/homoglyphs/"+target);
            break;
        case DOMAIN_IP:
            url.setUrl("https://api.binaryedge.io/v2/query/domains/ip/"+target);
            break;
        case DOMAIN_SEARCH:
            url.setUrl("https://api.binaryedge.io/v2/query/domains/search?query="+target);
            break;
        case DOMAIN_SUBDOMAIN:
            url.setUrl("https://api.binaryedge.io/v2/query/domains/subdomain/"+target);
            break;
        case HOST_HISTORICAL:
            url.setUrl("https://api.binaryedge.io/v2/query/ip/historical/"+target);
            break;
        case HOST_IP:
            url.setUrl("https://api.binaryedge.io/v2/query/ip/"+target);
            break;
        case HOST_SEARCH:
            url.setUrl("https://api.binaryedge.io/v2/query/search?query="+target);
            break;
        case HOST_SEARCHSTAT:
            url.setUrl("https://api.binaryedge.io/v2/query/search/stats?query="+target);
            break;
        case SUBSCRIPTION:
            url.setUrl("https://api.binaryedge.io/v2/user/subscription");
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        if(args.outputIp || args.outputSubdomainIp || args.outputSubdomain){
            url.setUrl("https://api.binaryedge.io/v2/query/ip/"+target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputDomain){
        if(args.outputSubdomain){
            url.setUrl("https://api.binaryedge.io/v2/query/domains/subdomain/"+target+"?page="+QString::number(m_page));
            request.setAttribute(QNetworkRequest::User, DOMAIN_SUBDOMAIN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputSubdomainIp || args.outputSubdomain || args.outputIp){
            url.setUrl("https://api.binaryedge.io/v2/query/domains/dns/"+target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void BinaryEdge::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray events = document.object()["events"].toArray();

    if(QUERY_TYPE == DOMAIN_SUBDOMAIN){
        foreach(const QJsonValue &value, events){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
    }

    /* domains/dns and domains/ip have results in same format */
    if(QUERY_TYPE == DOMAIN_DNS || QUERY_TYPE == DOMAIN_IP){
        foreach(const QJsonValue &value, events){
            QString domain = value.toObject()["domain"].toString();
            emit resultSubdomain(domain);
            log.resultsCount++;

            foreach(const QJsonValue &value, value.toObject()["NS"].toArray()){
                emit resultNS(value.toString());
                log.resultsCount++;
            }

            foreach(const QJsonValue &value, value.toObject()["MX"].toArray()){
                emit resultMX(value.toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void BinaryEdge::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray events = document.object()["events"].toArray();

    /* domains/dns and domains/ip have results in same format */
    if(QUERY_TYPE == DOMAIN_DNS || QUERY_TYPE == DOMAIN_IP){
        foreach(const QJsonValue &value, events){
            QString A = value.toObject()["A"].toArray().at(0).toString();
            QString AAAA = value.toObject()["AAAA"].toArray().at(0).toString();

            if(!A.isEmpty()){
                emit resultA(A);
                log.resultsCount++;
            }
            if(!AAAA.isEmpty()){
                emit resultAAAA(AAAA);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void BinaryEdge::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray events = document.object()["events"].toArray();

    /* domains/dns and domains/ip have results in same format */
    if(QUERY_TYPE == DOMAIN_DNS || QUERY_TYPE == DOMAIN_IP){
        foreach(const QJsonValue &value, events){
            QString domain = value.toObject()["domain"].toString();
            QString A = value.toObject()["A"].toArray().at(0).toString();
            QString AAAA = value.toObject()["AAAA"].toArray().at(0).toString();

            if(!A.isEmpty()){
                emit resultSubdomainIp(domain, A);
                log.resultsCount++;
            }
            if(!AAAA.isEmpty()){
                emit resultSubdomainIp(domain, AAAA);
                log.resultsCount++;
            }
        }
    }

    end(reply);
}
