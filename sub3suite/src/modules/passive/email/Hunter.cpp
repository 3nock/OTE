#include "Hunter.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ACCOUNT_INFO 0
#define AUTHOR_FINDER 1
#define DOMAIN_SEARCH 2
#define EMAIL_COUNT 3
#define EMAIL_FINDER 4
#define EMAIL_VERIFIER 5

Hunter::Hunter(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "Hunter";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Hunter::replyFinishedRawJson);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Hunter::replyFinishedEmail);
    ///
    /// getting api-key...
    ///
    
    m_key = APIKEY.value("hunter").toString();
    
}
Hunter::~Hunter(){
    delete manager;
}

void Hunter::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch(args.rawOption){
        case ACCOUNT_INFO:
            url.setUrl("https://api.hunter.io/v2/account?api_key="+m_key);
            break;
        case AUTHOR_FINDER:
            url.setUrl("https://api.hunter.io/v2/author-finder?url="+target+"&api_key="+m_key);
            break;
        case DOMAIN_SEARCH:
            url.setUrl("https://api.hunter.io/v2/domain-search?domain="+target+"&api_key="+m_key);
            break;
        case EMAIL_COUNT:
            url.setUrl("https://api.hunter.io/v2/email-count?domain="+target);
            break;
        case EMAIL_FINDER:
            url.setUrl("https://api.hunter.io/v2/email-finder?domain="+target+"&api_key="+m_key);
            break;
        case EMAIL_VERIFIER:
            url.setUrl("https://api.hunter.io/v2/email-verifier?email="+target+"&api_key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
    if(args.inputDomain){
        if(args.outputEmail){
            url.setUrl("https://api.hunter.io/v2/domain-search?domain="+target+"&api_key="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAIN_SEARCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Hunter::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document["data"].toObject();

    if(QUERY_TYPE == DOMAIN_SEARCH){
        /*
         * QString OrganizationName = data["organization"].toString();
         */
        QJsonArray emailList = data["emails"].toArray();
        foreach(const QJsonValue &value, emailList){
            QString Email = value["value"].toString();
            emit resultEmail(Email);
            log.resultsCount++;
            /*
             * getting where the email was extracted from
             *
            QStringList uriSources;
            foreach(const QJsonValue &sources, value["sources"].toArray())
                uriSources.append(value["uri"].toString());
             *
             */
        }
    }
    end(reply);
}
