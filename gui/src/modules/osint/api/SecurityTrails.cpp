#include "SecurityTrails.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ACCOUNT 0
#define ASSOCIATED_DOMAINS 1
#define ASSOCIATED_IPS 2
#define COMPANY 3
#define DNS_HISTORY 4
#define DOMAINS 5
#define IP_WHOIS 6
#define PING 7
#define SSL_CERT 8
#define SUBDOMAINS 9
#define WHOIS 10
#define WHOIS_HISTORY 11

/*
 * Limited to 2000 results for the Free plan and to 10000 for subdomainIp paid subscriptions.
 */
SecurityTrails::SecurityTrails(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "SecurityTrails";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &SecurityTrails::replyFinishedRaw);
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
    request.setRawHeader("apikey", m_key.toUtf8());
    request.setRawHeader("Accept", "application/json");

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case DNS_HISTORY:
            url.setUrl("https://api.securitytrails.com/v1/history/"+args->target+"/dns/any");
            break;
        case SUBDOMAINS:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/subdomains?children_only=false&include_inactive=true");
            break;
        case WHOIS_HISTORY:
            url.setUrl("https://api.securitytrails.com/v1/history/"+args->target+"/whois");
            break;
        case WHOIS:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/whois");
            break;
        case ASSOCIATED_DOMAINS:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/associated");
            break;
        case ASSOCIATED_IPS:
            url.setUrl("https://api.securitytrails.com/v1/company/"+args->target+"/associated-ips");
            break;
        case DOMAINS:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target);
            break;
        case COMPANY:
            url.setUrl("https://api.securitytrails.com/v1/company/"+args->target);
            break;
        case SSL_CERT:
            url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/ssl?include_subdomains=false&status=valid");
            break;
        case IP_WHOIS:
            url.setUrl("https://api.securitytrails.com/v1/ips/"+args->target+"/whois");
            break;
        case PING:
            url.setUrl("https://api.securitytrails.com/v1/ping");
            break;
        case ACCOUNT:
            url.setUrl("https://api.securitytrails.com/v1/account/usage");
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputIp){
        url.setUrl("https://api.securitytrails.com/v1/domain/"+args->target+"/subdomains?children_only=false&include_inactive=true");
    }

    if(args->inputDomain){

    }
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
            emit subdomain(value.toString().append(".").append(args->target));
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
