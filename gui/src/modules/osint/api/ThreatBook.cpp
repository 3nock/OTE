#include "ThreatBook.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DOMAIN_QUERY 0
#define IP_QUERY 1
#define SUBDOMAINS 2


ThreatBook::ThreatBook(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "ThreatBook";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &ThreatBook::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &ThreatBook::replyFinishedSubdomain);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("threatbook").toString();
    Config::generalConfig().endGroup();
}
ThreatBook::~ThreatBook(){
    delete manager;
}

void ThreatBook::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case SUBDOMAINS:
            url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+args->target);
            break;
        case DOMAIN_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/domain/query?apikey="+m_key+"&resource="+args->target);
            break;
        case IP_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/ip/query?apikey="+m_key+"&resource="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+args->target);
            request.setAttribute(QNetworkRequest::User, SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputIp){

    }
}

void ThreatBook::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == SUBDOMAINS){
        QJsonArray subdomainList = document.object()["data"].toObject()["sub_domains"].toObject()["data"].toArray();
        foreach(const QJsonValue &value, subdomainList){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
    }
    end(reply);
}
