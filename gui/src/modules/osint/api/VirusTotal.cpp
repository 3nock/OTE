#include "VirusTotal.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "src/utils/Config.h"


VirusTotal::VirusTotal(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &VirusTotal::replyFinished);
    ///
    /// obtain apikey...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("virustotalapi").toString();
    Config::generalConfig().endGroup();
}
VirusTotal::~VirusTotal(){
    delete manager;
}

void VirusTotal::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "urls")
            url.setUrl("https://www.virustotal.com/api/v3/urls/"+args->target);
        if(args->option == "domains")
            url.setUrl("https://www.virustotal.com/api/v3/domains/"+args->target);
        if(args->option == "resolutions")
            url.setUrl("https://www.virustotal.com/api/v3/resolutions/"+args->target);
        if(args->option == "ip-addresses")
            url.setUrl("https://www.virustotal.com/api/v3/ip_addresses/"+args->target);

        request.setUrl(url);
        request.setRawHeader("x-apikey", m_key.toUtf8());
        manager->get(request);

    }else{
        url.setUrl("https://www.virustotal.com/vtapi/v2/domain/report?apikey="+m_key+"&domain="+args->target);
        request.setUrl(url);
        manager->get(request);
    }
}

void VirusTotal::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
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
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
