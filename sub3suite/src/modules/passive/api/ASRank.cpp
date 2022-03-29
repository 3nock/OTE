/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "ASRank.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASN 0
#define ASN_ASN 1
#define ASN_NAME 2
#define LINK 3
#define LINK_LINK 4
#define ORG 5
#define ORG_ORG 6

ASRank::ASRank(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_ASRANK;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ASRank::replyFinishedRawJson);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ASRank::replyFinishedAsn);
}
ASRank::~ASRank(){
    delete manager;
}

void ASRank::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case ASN:
            url.setUrl("https://api.asrank.caida.org/v2/restful/asns");
            break;
        case ASN_ASN:
            url.setUrl("https://api.asrank.caida.org/v2/restful/asns/"+target);
            break;
        case ASN_NAME:
            url.setUrl("https://api.asrank.caida.org/v2/restful/asns/name/"+target);
            break;
        case LINK:
            url.setUrl("https://api.asrank.caida.org/v2/restful/asnLinks");
            break;
        case LINK_LINK:
            url.setUrl("https://api.asrank.caida.org/v2/restful/asnLinks/"+target);
            break;
        case ORG:
            url.setUrl("https://api.asrank.caida.org/v2/restful/organizations");
            break;
        case ORG_ORG:
            url.setUrl("https://api.asrank.caida.org/v2/restful/organizations/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Search){
        if(args.output_ASN){
            url.setUrl("https://api.asrank.caida.org/v2/restful/asns/name/"+target);
            request.setAttribute(QNetworkRequest::User, ASN_NAME);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void ASRank::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray edges = document.object()["data"].toObject()["asns"].toObject()["edges"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASN_NAME:
        foreach(const QJsonValue &value, edges){
            QJsonObject node = value.toObject()["node"].toObject();
            emit resultASN(node["asn"].toString(), node["asnName"].toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}
