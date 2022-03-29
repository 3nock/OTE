#include "Waybackmachine.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define MATCHTYPE_DOMAIN 0
#define MATCHTYPE_EXACT 1
#define MATCHTYPE_HOST 2
#define MATCHTYPE_PREFIX 3

/*
 *  &from=2010&to=2018
 *   &limit=999999
 */

Waybackmachine::Waybackmachine(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_WAYBACKMACHINE;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Waybackmachine::replyFinishedRawJson);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Waybackmachine::replyFinishedUrl);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Waybackmachine::replyFinishedSubdomain);
}
Waybackmachine::~Waybackmachine(){
    delete manager;
}

void Waybackmachine::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case MATCHTYPE_DOMAIN:
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=domain&output=json&collapse=urlkey&url="+target);
            break;
        case MATCHTYPE_EXACT:
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=exact&output=json&collapse=urlkey&url="+target);
            break;
        case MATCHTYPE_HOST:
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=host&output=json&collapse=urlkey&url="+target);
            break;
        case MATCHTYPE_PREFIX:
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=prefix&output=json&collapse=urlkey&url="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_URL){
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=prefix&fl=original&output=json&collapse=urlkey&url="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_Hostname){
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=domain&fl=original&output=json&collapse=urlkey&url="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_URL){
        if(args.output_URL){
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=prefix&fl=original&output=json&collapse=urlkey&url="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_Hostname){
            url.setUrl("https://web.archive.org/cdx/search/cdx?matchType=domain&fl=original&output=json&collapse=urlkey&url="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void Waybackmachine::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
    QJsonArray urls = jsonReply.array();

    urls.removeFirst();
    foreach(const QJsonValue &value, urls)
    {
        QString urlValue = value.toArray()[0].toString();
        emit resultURL(urlValue);
        log.resultsCount++;
    }

    this->end(reply);
}

void Waybackmachine::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
    QJsonArray mainArray = jsonReply.array();

    mainArray.removeFirst();
    foreach(const QJsonValue &value, mainArray)
    {
        /* getting url */
        QString domainUrl = value.toArray()[0].toString();

        /* extracting subdomain from url...*/
        domainUrl.remove("http://");
        domainUrl.remove("https://");
        domainUrl = domainUrl.split("/").at(0);

        /* remove port number if available */
        if(domainUrl.contains(":")){
            domainUrl = domainUrl.split(":").at(0);
        }

        /*  emiting subdomain... */
        emit resultSubdomain(domainUrl);
        log.resultsCount++;
    }

    this->end(reply);
}
