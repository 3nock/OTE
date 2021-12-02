#include "Onyphe.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define SIMPLE_BEST_GEOLOC 0
#define SIMPLE_BEST_INETNUM 1
#define SIMPLE_BEST_THREATLIST 2
#define SIMPLE_CTL 3
#define SIMPLE_DATAMD5 4
#define SIMPLE_DATASCAN 5
#define SIMPLE_E_DATASHOT 6
#define SIMPLE_E_ONIONSCAN 7
#define SIMPLE_E_ONIONSHOT 8
#define SIMPLE_E_TOPSITE 9
#define SIMPLE_E_VULNSCAN 10
#define SIMPLE_E_WHOIS 11
#define SIMPLE_FORWARD 12
#define SIMPLE_GEOLOC 13
#define SIMPLE_INETNUM 14
#define SIMPLE_PASTRIES 15
#define SIMPLE_RESOLVER 16
#define SIMPLE_REVERSE 17
#define SIMPLE_SNIFFER 18
#define SIMPLE_SYNSCAN 19
#define SIMPLE_THREATLIST 20
#define SIMPLE_E_BEST_WHOIS 21
#define SUMMARY_DOMAIN 22
#define SUMMARY_HOSTNAME 23
#define SUMMARY_IP 24
#define USER 25

/*
 * alot not yet implemented...
 * 2500 queries per month for free-api
 */
Onyphe::Onyphe(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Onyphe";

    if(args->outputRaw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Onyphe::replyFinishedRawJson);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Onyphe::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Onyphe::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Onyphe::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &Onyphe::replyFinishedAsn);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("onyphe").toString();
    Config::generalConfig().endGroup();
}
Onyphe::~Onyphe(){
    delete manager;
}

void Onyphe::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "apikey "+m_key.toUtf8());

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case USER:
            url.setUrl("https://www.onyphe.io/api/v2/user");
            break;
        case SUMMARY_IP:
            url.setUrl("https://www.onyphe.io/api/v2/summary/ip/"+args->target);
            break;
        case SUMMARY_DOMAIN:
            url.setUrl("https://www.onyphe.io/api/v2/summary/domain/"+args->target);
            break;
        case SUMMARY_HOSTNAME:
            url.setUrl("https://www.onyphe.io/api/v2/summary/hostname/"+args->target);
            break;
        case SIMPLE_GEOLOC:
            url.setUrl("https://www.onyphe.io/api/v2/simple/geoloc/"+args->target);
            break;
        case SIMPLE_INETNUM:
            url.setUrl("https://www.onyphe.io/api/v2/simple/inetnum/"+args->target);
            break;
        case SIMPLE_PASTRIES:
            url.setUrl("https://www.onyphe.io/api/v2/simple/pastries/"+args->target);
            break;
        case SIMPLE_RESOLVER:
            url.setUrl("https://www.onyphe.io/api/v2/simple/resolver/"+args->target);
            break;
        case SIMPLE_SNIFFER:
            url.setUrl("https://www.onyphe.io/api/v2/simple/sniffer/"+args->target);
            break;
        case SIMPLE_SYNSCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/synscan/"+args->target);
            break;
        case SIMPLE_THREATLIST:
            url.setUrl("https://www.onyphe.io/api/v2/simple/threatlist/"+args->target);
            break;
        case SIMPLE_CTL:
            url.setUrl("https://www.onyphe.io/api/v2/simple/ctl/"+args->target);
            break;
        case SIMPLE_DATASCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/datascan/"+args->target);
            break;
        case SIMPLE_DATAMD5:
            url.setUrl("https://www.onyphe.io/api/v2/simple/datascan/datamd5/"+args->target);
            break;
        case SIMPLE_FORWARD:
            url.setUrl("https://www.onyphe.io/api/v2/simple/resolver/forward/"+args->target);
            break;
        case SIMPLE_REVERSE:
            url.setUrl("https://www.onyphe.io/api/v2/simple/resolver/reverse/"+args->target);
            break;
        case SIMPLE_E_DATASHOT:
            url.setUrl("https://www.onyphe.io/api/v2/simple/datashot/"+args->target);
            break;
        case SIMPLE_E_ONIONSHOT:
            url.setUrl("https://www.onyphe.io/api/v2/simple/onionshot/"+args->target);
            break;
        case SIMPLE_E_TOPSITE:
            url.setUrl("https://www.onyphe.io/api/v2/simple/topsite/"+args->target);
            break;
        case SIMPLE_E_VULNSCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/vulnscan/"+args->target);
            break;
        case SIMPLE_E_WHOIS:
            url.setUrl("https://www.onyphe.io/api/v2/simple/whois/"+args->target);
            break;
        case SIMPLE_E_ONIONSCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/onionscan/"+args->target);
            break;
        case SIMPLE_E_BEST_WHOIS:
            url.setUrl("https://www.onyphe.io/api/v2/simple/whois/best/"+args->target);
            break;
        case SIMPLE_BEST_GEOLOC:
            url.setUrl("https://www.onyphe.io/api/v2/simple/geoloc/best/"+args->target);
            break;
        case SIMPLE_BEST_INETNUM:
            url.setUrl("https://www.onyphe.io/api/v2/simple/inetnum/best/"+args->target);
            break;
        case SIMPLE_BEST_THREATLIST:
            url.setUrl("https://www.onyphe.io/api/v2/simple/threatlist/best/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Onyphe::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}

void Onyphe::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}

void Onyphe::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}

void Onyphe::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}
