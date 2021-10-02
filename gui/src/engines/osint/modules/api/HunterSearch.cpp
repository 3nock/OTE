#include "HunterSearch.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 *
 * is an email finder and verifier...
 * find email on a domain using first and last name
 *          https://api.hunter.io/v2/email-finder?domain=reddit.com&first_name=Alexis&last_name=Ohanian&api_key=974523c651df0e318c73b26bc316b2c3c5c27163
 * to verify email:
 *          https://api.hunter.io/v2/email-verifier?email=patrick@stripe.com&api_key=974523c651df0e318c73b26bc316b2c3c5c27163
 *
 */

HunterSearch::HunterSearch(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
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
    QUrl url("https://api.hunter.io/v2/domain-search?domain="+target+"&api_key="+m_key);
    request.setUrl(url);
    manager->get(request);
}

void HunterSearch::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = jsonReply["data"].toObject();
        /*
         * QString OrganizationName = data["organization"].toString();
         */
        QJsonArray emailList = data["emails"].toArray();
        foreach(const QJsonValue &value, emailList){
            QString email = value["value"].toString();
            emit scanResults(email);
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
