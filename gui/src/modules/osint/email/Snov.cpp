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
 * still many feautures arent implemented
 */
Snov::Snov(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Snov";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Snov::replyFinishedRaw);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("snov").toString();
    Config::generalConfig().endGroup();
}
Snov::~Snov(){
    delete manager;
}

void Snov::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case CHECK_USER_BALANCE:
        {
            url.setUrl("https://api.snov.io/v1/get-balance");
            request.setUrl(url);
            manager->get(request);
            break;
        }
        case DOMAIN_SEARCH:
        {
            url.setUrl("https://api.snov.io/v2/domain-emails-with-info?type=all&access_token="+m_key+"&domain="+args->target);
            request.setUrl(url);
            manager->get(request);
            break;
        }
        case EMAIL_COUNT:
        {
            url.setUrl("https://api.snov.io/v1/get-domain-emails-count");
            QByteArray data;
            data.append("access_token="+m_key+"&");
            data.append("domain="+args->target);
            request.setUrl(url);
            manager->post(request, data);
            break;
        }
        case EMAIL_VERIFIER:
        {
            url.setUrl("https://api.snov.io/v1/get-emails-verification-status?emails[]="+args->target);
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
            data.append("email="+args->target);
            request.setUrl(url);
            manager->post(request, data);
        }
        activeRequests++;
    }
}

void Snov::replyFinishedEmail(QNetworkReply *reply){
    Q_UNUSED(reply);
}
