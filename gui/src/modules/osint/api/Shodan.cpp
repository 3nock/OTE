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

    QUrl url;
    if(args->raw){
        if(args->option == "host ip")
            url.setUrl("https://api.shodan.io/shodan/host/"+args->target+"?key="+m_key);
        if(args->option == "host count")
            url.setUrl("https://api.shodan.io/shodan/host/count?key="+m_key+"&query="+args->target+"&facets=org,os");
        if(args->option == "host search")
            url.setUrl("https://api.shodan.io/shodan/host/search?key="+m_key+"&query="+args->target+"&facets=country");
        if(args->option == "host search facets")
            url.setUrl("https://api.shodan.io/shodan/host/search/facets?key="+m_key);
        if(args->option == "host search filters")
            url.setUrl("https://api.shodan.io/shodan/host/search/filters?key="+m_key);
        if(args->option == "host search tokens")
            url.setUrl("https://api.shodan.io/shodan/host/search/tokens?key="+m_key+"&query="+args->target);
        if(args->option == "bulk data")
            url.setUrl("https://api.shodan.io/shodan/data/"+args->target+"?key="+m_key);
        if(args->option == "account profile")
            url.setUrl("https://api.shodan.io/account/profile?key="+m_key);
        if(args->option == "dns domain")
            url.setUrl("https://api.shodan.io/dns/domain/"+args->target+"?key="+m_key);
        if(args->option == "dns resolve")
            url.setUrl("https://api.shodan.io/dns/resolve?hostnames="+args->target+"&key="+m_key);
        if(args->option == "dns reverse")
            url.setUrl("https://api.shodan.io/dns/reverse?ips="+args->target+"&key="+m_key);
    }else{
        url.setUrl("https://api.shodan.io/dns/domain/"+args->target+"?key="+m_key);
    }

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
                emit subdomain(hostname);
        }
        foreach(const QJsonValue &value, subdomains)
            emit subdomain(value.toString());
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
