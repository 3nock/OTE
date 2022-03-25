/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Anubis.h"
#include <QJsonDocument>
#include <QJsonArray>


#define SUBDOMAIN 0

/*
 *  https://jonlu.ca/anubis/subdomains/
 *  https://jldc.me/anubis/subdomains/
 */
Anubis::Anubis(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_ANUBIS;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Anubis::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Anubis::replyFinishedSubdomain);
}
Anubis::~Anubis(){
    delete manager;
}

void Anubis::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case SUBDOMAIN:
            url.setUrl("https://jldc.me/anubis/subdomains/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.input_Domain){
        url.setUrl("https://jldc.me/anubis/subdomains/"+target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Anubis::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array()){
        emit resultSubdomain(value.toString());
        log.resultsCount++;
    }

    end(reply);
}
