#include "ZETAlytics.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/* Not well implemented yet...*/

ZETAlytics::ZETAlytics(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ZETAlytics::replyFinished);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("zetalytics").toString();
    Config::generalConfig().endGroup();
}
ZETAlytics::~ZETAlytics(){
    delete manager;
}

void ZETAlytics::start(){
    QNetworkRequest request;
    QUrl url("https://zonecruncher.com/api/v1/subdomains?q="+target+"&token="+m_key);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void ZETAlytics::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        /*
         * Not implemented yet...
         */
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
