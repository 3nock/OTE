#include "Projectdiscovery.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define SUBDOMAIN 0


Projectdiscovery::Projectdiscovery(ScanArgs args):
    AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_PROJECTDISCOVERY;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Projectdiscovery::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Projectdiscovery::replyFinishedSubdomain);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_PROJECTDISCOVERY).toString();
}
Projectdiscovery::~Projectdiscovery(){
    delete manager;
}

void Projectdiscovery::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Authorization", m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case SUBDOMAIN:
            url.setUrl("https://dns.projectdiscovery.io/dns/"+target+"/subdomains");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://dns.projectdiscovery.io/dns/"+target+"/subdomains");
            request.setAttribute(QNetworkRequest::User, SUBDOMAIN);
            request.setUrl(url);
            manager->get(request);
        }
    }
}

void Projectdiscovery::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    QJsonArray subdomainList = document.object()["subdomains"].toArray();
    foreach(const QJsonValue &value, subdomainList){
        emit resultSubdomain(value.toString());
        log.resultsCount++;
    }

    this->end(reply);
}
