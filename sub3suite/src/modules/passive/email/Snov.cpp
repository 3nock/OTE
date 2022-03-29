#include "Snov.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define CHECK_USER_BALANCE 0
#define DOMAIN_SEARCH 1
#define EMAIL_COUNT 2
#define EMAIL_VERIFIER 3
#define GET_PROFILE_WITH_EMAIL 4

/*
 * Many api not implemented
 */
Snov::Snov(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_SNOV;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Snov::replyFinishedRawJson);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Snov::replyFinishedEmail);

    /* getting api-key */
    m_key = APIKEY.value(OSINT_MODULE_SNOV).toString();
}
Snov::~Snov(){
    delete manager;
}

void Snov::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case CHECK_USER_BALANCE:
        {
            url.setUrl("https://api.snov.io/v1/get-balance");
            request.setUrl(url);
            manager->get(request);
            break;
        }
        case DOMAIN_SEARCH:
        {
            url.setUrl("https://api.snov.io/v2/domain-emails-with-info?type=all&access_token="+m_key+"&domain="+target);
            request.setUrl(url);
            manager->get(request);
            break;
        }
        case EMAIL_COUNT:
        {
            url.setUrl("https://api.snov.io/v1/get-domain-emails-count");
            QByteArray data;
            data.append("access_token="+m_key+"&");
            data.append("domain="+target);
            request.setUrl(url);
            manager->post(request, data);
            break;
        }
        case EMAIL_VERIFIER:
        {
            url.setUrl("https://api.snov.io/v1/get-emails-verification-status?emails[]="+target);
            QByteArray data;
            data.append("access_token="+m_key);
            request.setUrl(url);
            manager->post(request, data);
            break;
        }
        case GET_PROFILE_WITH_EMAIL:
            url.setUrl("https://api.snov.io/v1/get-profile-by-email");
            QByteArray data;
            data.append("access_token="+m_key+"&");
            data.append("email="+target);
            request.setUrl(url);
            manager->post(request, data);
        }
        return;
    }

    if(args.input_Domain){
        if(args.output_Email){
            url.setUrl("https://api.snov.io/v2/domain-emails-with-info?type=all&access_token="+m_key+"&domain="+target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_SEARCH);
            request.setUrl(url);
            manager->get(request);
        }
    }
}

void Snov::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_SEARCH:
        foreach(const QJsonValue &email, document.object()["emails"].toArray()){
            emit resultEmail(email["email"].toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}
