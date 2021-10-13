#include "SecurityTrails.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * Limited to 2000 results for the Free plan and to 10000 for all paid subscriptions.
 */
SecurityTrails::SecurityTrails(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &SecurityTrails::replyFinished);
    ///
    /// get api key....
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("securitytrails").toString();
    Config::generalConfig().endGroup();
}
SecurityTrails::~SecurityTrails(){
    delete manager;
}

void SecurityTrails::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "dns history")
            url.setUrl("https://api.securitytrails.com/v1/history/"+args->target+"/dns/any");
        if(args->option == "subdomains")
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/subdomains?children_only=false&include_inactive=true");
        if(args->option == "whois history")
            url.setUrl("https://api.securitytrails.com/v1/history/"+args->target+"/whois");
        if(args->option == "whois")
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/whois");
        if(args->option == "associated domains")
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/associated");
        if(args->option == "associated ips")
            url.setUrl("https://api.securitytrails.com/v1/company/"+args->target+"/associated-ips");
        if(args->option == "domain")
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target);
        if(args->option == "company")
            url.setUrl("https://api.securitytrails.com/v1/company/"+args->target);
        if(args->option == "ssl cert")
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/ssl?include_subdomains=false&status=valid");
        if(args->option == "ip whois")
            url.setUrl("https://api.securitytrails.com/v1/ips/"+args->target+"/whois");
        if(args->option == "ping")
            url.setUrl("https://api.securitytrails.com/v1/ping");
        if(args->option == "account")
            url.setUrl("https://api.securitytrails.com/v1/account/usage");
    }
    else{
        url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/subdomains?children_only=false&include_inactive=true");
    }

    request.setUrl(url);
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("apikey", m_key.toUtf8());
    manager->get(request);
}

void SecurityTrails::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString().append(".").append(args->target));
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
