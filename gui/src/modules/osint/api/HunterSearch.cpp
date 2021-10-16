#include "HunterSearch.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


HunterSearch::HunterSearch(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HunterSearch::replyFinished);
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
        if(args->option == "Domain Search")
            url.setUrl("https://api.hunter.io/v2/domain-search?domain="+args->target+"&api_key="+m_key);
        if(args->option == "Email Finder")
            url.setUrl("https://api.hunter.io/v2/email-finder?domain="+args->target+"&api_key="+m_key);
        if(args->option == "Author Finder")
            url.setUrl("https://api.hunter.io/v2/author-finder?url="+args->target+"&api_key="+m_key);
        if(args->option == "Email Verifier")
            url.setUrl("https://api.hunter.io/v2/email-verifier?email="+args->target+"&api_key="+m_key);
        if(args->option == "Email Count")
            url.setUrl("https://api.hunter.io/v2/email-count?domain="+args->target);
        if(args->option == "Account Information")
            url.setUrl("https://api.hunter.io/v2/account?api_key="+m_key);
    }else{
        url.setUrl("https://api.hunter.io/v2/domain-search?domain="+args->target+"&api_key="+m_key);
    }

    request.setUrl(url);
    manager->get(request);
}

void HunterSearch::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = jsonReply["data"].toObject();
        /*
         * QString OrganizationName = data["organization"].toString();
         */
        QJsonArray emailList = data["emails"].toArray();
        foreach(const QJsonValue &value, emailList){
            QString email = value["value"].toString();
            emit subdomain(email);
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
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

bool HunterSearch::verifyEmail(QString email){
    Q_UNUSED(email);
    return true;
}
