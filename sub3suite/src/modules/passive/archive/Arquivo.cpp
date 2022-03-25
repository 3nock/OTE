#include "Arquivo.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define CDX_SERVER 0
#define TEXTSEARCH 1

/*
 * for now use *. hence data on subdomains
 * cdx uses ndjson...
 * maxItems set to 100...
 */
Arquivo::Arquivo(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_ARQUIVO;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Arquivo::replyFinishedRawJson);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Arquivo::replyFinishedUrl);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Arquivo::replyFinishedSubdomain);
}
Arquivo::~Arquivo(){
    delete manager;
}

void Arquivo::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case CDX_SERVER:
            url.setUrl("https://arquivo.pt/wayback/cdx?url="+target+"&output=json");
            break;
        case TEXTSEARCH:
            url.setUrl("https://arquivo.pt/textsearch?q="+target+"&prettyPrint=false&maxItems=100");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.input_Domain){
        if(args.output_URL){
            url.setUrl("https://arquivo.pt/textsearch?q="+target+"/*&prettyPrint=false&maxItems=100");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.output_Hostname){
            url.setUrl("https://arquivo.pt/textsearch?q=*."+target+"&prettyPrint=false&maxItems=100");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_URL){
        if(args.output_Hostname){
            url.setUrl("https://arquivo.pt/textsearch?q="+target+"&prettyPrint=false&maxItems=100");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.output_URL){
            url.setUrl("https://arquivo.pt/textsearch?q="+target+"*&prettyPrint=false&maxItems=100");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_Search){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://arquivo.pt/textsearch?q="+target+"&prettyPrint=false&maxItems=100");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Arquivo::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray response_items = document.object()["response_items"].toArray();

    foreach(const QJsonValue &response_item, response_items){
        QString urlValue = response_item.toObject()["originalURL"].toString();
        emit resultURL(urlValue);
        log.resultsCount++;
    }

    end(reply);
}

void Arquivo::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray response_items = document.object()["response_items"].toArray();

    foreach(const QJsonValue &response_item, response_items)
    {
        /* getting url */
        QString domainUrl = response_item.toObject()["originalURL"].toString();

        /* extracting subdomain from url...*/
        domainUrl.remove("http://");
        domainUrl.remove("https://");
        domainUrl = domainUrl.split("/").at(0);

        /*  emiting subdomain... */
        emit resultSubdomain(domainUrl);
        log.resultsCount++;
    }

    end(reply);
}
