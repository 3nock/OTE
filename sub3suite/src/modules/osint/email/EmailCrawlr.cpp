#include "EmailCrawlr.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ACCOUNT 0
#define DOMAIN_LOOKUP 1
#define EMAIL_LOOKUP 2
#define EMAIL_VERIFICATION 3

/*
 * returns a BadGateway Error
 */
EmailCrawlr::EmailCrawlr(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "EmailCrawlr";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &EmailCrawlr::replyFinishedRawJson);
    if(args->outputEmail)
        connect(manager, &NetworkAccessManager::finished, this, &EmailCrawlr::replyFinishedEmail);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("emailcrawlr").toString();
    Config::generalConfig().endGroup();
}
EmailCrawlr::~EmailCrawlr(){
    delete manager;
}

void EmailCrawlr::start(){
    QNetworkRequest request;
    request.setRawHeader("x-api-key", m_key.toUtf8());

    QUrl url;
    if(args->outputRaw){
        switch(args->rawOption){
        case ACCOUNT:
            url.setUrl("https://api.emailcrawlr.com/v2/account");
            break;
        case DOMAIN_LOOKUP:
            url.setUrl("https://api.emailcrawlr.com/v2/domain?domain="+args->target);
            break;
        case EMAIL_LOOKUP:
            url.setUrl("https://api.emailcrawlr.com/v2/email?email="+args->target);
            break;
        case EMAIL_VERIFICATION:
            url.setUrl("https://api.emailcrawlr.com/v2/email/verify?email="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://api.emailcrawlr.com/v2/domain?domain="+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void EmailCrawlr::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray emails = document.object()["emails"].toArray();

    foreach(const QJsonValue &value, emails){
        QString mail = value.toObject()["email"].toString();
        emit email(mail);
        log.resultsCount++;
    }

    end(reply);
}
