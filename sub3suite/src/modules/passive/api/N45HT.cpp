#include "N45HT.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define SUBDOMAIN 0

/*
 * havent seen the api documentation yet...
 */
N45HT::N45HT(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_N45HT;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &N45HT::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &N45HT::replyFinishedSubdomain);
}
N45HT::~N45HT(){
    delete manager;
}

void N45HT::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case SUBDOMAIN:
            url.setUrl("https://api.n45ht.or.id/v1/subdomain-enumeration?domain="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        url.setUrl("https://api.n45ht.or.id/v1/subdomain-enumeration?domain="+target);
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

void N45HT::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray subdomains = document.object()["subdomains"].toArray();

    foreach(const QJsonValue &value, subdomains){
        emit resultSubdomain(value.toString());
        log.resultsCount++;
    }

    this->end(reply);
}
