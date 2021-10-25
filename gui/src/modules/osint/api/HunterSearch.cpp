#include "HunterSearch.h"
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

HunterSearch::HunterSearch(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Hunter";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &HunterSearch::replyFinishedRaw);
    if(args->outputEmail)
        connect(manager, &MyNetworkAccessManager::finished, this, &HunterSearch::replyFinishedEmail);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("huntersearch").toString();
    Config::generalConfig().endGroup();
}
HunterSearch::~HunterSearch(){
    delete manager;
}

void HunterSearch::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case ACCOUNT_INFO:
            url.setUrl("https://api.hunter.io/v2/account?api_key="+m_key);
            break;
        case AUTHOR_FINDER:
            url.setUrl("https://api.hunter.io/v2/author-finder?url="+args->target+"&api_key="+m_key);
            break;
        case DOMAIN_SEARCH:
            url.setUrl("https://api.hunter.io/v2/domain-search?domain="+args->target+"&api_key="+m_key);
            break;
        case EMAIL_COUNT:
            url.setUrl("https://api.hunter.io/v2/email-count?domain="+args->target);
            break;
        case EMAIL_FINDER:
            url.setUrl("https://api.hunter.io/v2/email-finder?domain="+args->target+"&api_key="+m_key);
            break;
        case EMAIL_VERIFIER:
            url.setUrl("https://api.hunter.io/v2/email-verifier?email="+args->target+"&api_key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
    if(args->inputDomain){
        if(args->outputEmail){
            url.setUrl("https://api.hunter.io/v2/domain-search?domain="+args->target+"&api_key="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAIN_SEARCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void HunterSearch::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = jsonReply["data"].toObject();

        if(requestType == DOMAIN_SEARCH){
            /*
             * QString OrganizationName = data["organization"].toString();
             */
            QJsonArray emailList = data["emails"].toArray();
            foreach(const QJsonValue &value, emailList){
                QString Email = value["value"].toString();
                emit email(Email);
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
    }
    end(reply);
}

bool HunterSearch::verifyEmail(QString email){
    Q_UNUSED(email);
    return true;
}
