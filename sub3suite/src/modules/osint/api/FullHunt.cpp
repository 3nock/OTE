#include "FullHunt.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DOMAIN_DETAILS 0
#define DOMAIN_SUBDOMAINS 1
#define HOST_DETAILS 2

/*
 * https://api-docs.fullhunt.io/#rate-limiting
 * some error with domain queries...
 */
FullHunt::FullHunt(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "FullHunt";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &FullHunt::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &FullHunt::replyFinishedSubdomain);
    ///
    /// getting api key....
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("fullhunt").toString();
    Config::generalConfig().endGroup();
}
FullHunt::~FullHunt(){
    delete manager;
}

void FullHunt::start(){
    QNetworkRequest request;
    request.setRawHeader("X-API-KEY", m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case DOMAIN_DETAILS:
            url.setUrl("https://fullhunt.io/api/v1/domain/"+target+"/details");
            break;
        case DOMAIN_SUBDOMAINS:
            url.setUrl("https://fullhunt.io/api/v1/domain/"+target+"/subdomains");
            break;
        case HOST_DETAILS:
            url.setUrl("https://fullhunt.io/api/v1/host/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomain){
            QUrl url("https://jldc.me/FullHunt/subdomains/"+target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void FullHunt::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(QUERY_TYPE == DOMAIN_SUBDOMAINS){
        QJsonArray subdomains = document.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomains){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

