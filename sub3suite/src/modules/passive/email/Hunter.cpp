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
    log.moduleName = OSINT_MODULE_HUNTER;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Hunter::replyFinishedRawJson);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Hunter::replyFinishedEmail);

    /* getting api-key */
    m_key = APIKEY.value(OSINT_MODULE_HUNTER).toString();
}
Hunter::~Hunter(){
    delete manager;
}

void Hunter::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch(args.raw_query_id){
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
    if(args.input_Domain){
        if(args.output_Email){
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_SEARCH:
        foreach(const QJsonValue &value, data["emails"].toArray()){
            emit resultEmail(value["value"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
