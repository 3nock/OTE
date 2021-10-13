#include "Shodan.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * 1 request/ second
 */
Shodan::Shodan(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Shodan::replyFinished);
    ///
    /// get api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("shodan").toString();
    Config::generalConfig().endGroup();
}
Shodan::~Shodan(){
    delete manager;
}

void Shodan::start(){
    QNetworkRequest request;
    QUrl url("https://api.shodan.io/dns/domain/"+args->target+"?key="+m_key);
    request.setUrl(url);
    manager->get(request);
}

void Shodan::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray data = jsonReply.object()["data"].toArray();
        QJsonArray subdomains = jsonReply.object()["subdomains"].toArray();
        ///
        /// for subdomains...
        ///
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            QString hostname = value.toObject()["value"].toString();
            if(type == "MX" || type == "NS" || type == "CNAME")
                emit scanResults(hostname);
        }
        foreach(const QJsonValue &value, subdomains)
            emit scanResults(value.toString());
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
