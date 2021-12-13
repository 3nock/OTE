#include "Projectdiscovery.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define SUBDOMAIN 0

Projectdiscovery::Projectdiscovery(ScanArgs args):
    AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "ProjectDiscovery";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Projectdiscovery::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Projectdiscovery::replyFinishedSubdomain);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("projectdiscovery").toString();
    Config::generalConfig().endGroup();
}
Projectdiscovery::~Projectdiscovery(){
    delete manager;
}

void Projectdiscovery::start(){
    QNetworkRequest request;
    request.setRawHeader("Authorization", m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch(args.rawOption){
        case SUBDOMAIN:
            url.setUrl("https://dns.projectdiscovery.io/dns/"+target+"/subdomains");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        url.setUrl("https://dns.projectdiscovery.io/dns/"+target+"/subdomains");
        request.setAttribute(QNetworkRequest::User, SUBDOMAIN);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Projectdiscovery::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObject = document.object();
    QString error = mainObject["error"].toString();
    if(error.isNull() || error.isEmpty()){
        QJsonArray subdomainList = mainObject["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomainList){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
    }
    end(reply);
}
