#include "VirusTotal.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "src/Config.h"

VirusTotal::VirusTotal(QString _target, QObject *parent):
    AbstractModule (parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &VirusTotal::replyFinished);
}
VirusTotal::~VirusTotal(){
    delete manager;
}

void VirusTotal::start(){
    ///
    /// obtain apikey...
    ///
    Config::settings().beginGroup("api-keys");
    QString apikey = Config::settings().value("virustotalapi").toString();
    Config::settings().endGroup();
    ///
    /// sending request...
    ///
    QNetworkRequest request;
    QUrl url("https://www.virustotal.com/vtapi/v2/domain/report?apikey="+apikey+"&domain="+target);
    request.setUrl(url);
    manager->get(request);
}

void VirusTotal::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        ///
        /// for subdomains...
        ///
        QJsonArray subdomains = jsonObject["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomains)
            emit scanResults(value.toString());
        /*
        ///
        /// ip-addresses...
        ///
        QJsonArray resolutions = jsonObject["resolutions"].toArray();
        foreach(const QJsonValue &value, resolutions)
            emit scanResults(value.toObject()["ip_address"].toString());
        ///
        /// detected-urls...
        ///
        QJsonArray urls = jsonObject["detected_urls"].toArray();
        foreach(const QJsonValue &value, urls)
            emit scanResults(value.toObject()["url"].toString());
        */
    }
    else
    {
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
