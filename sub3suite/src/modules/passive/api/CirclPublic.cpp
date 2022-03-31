#include "CirclPublic.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define IP_2_ASN 0

/*
 * many features not implemented
 */
CirclPublic::CirclPublic(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_CIRCLPUBLIC;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CirclPublic::replyFinishedRawJson);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CirclPublic::replyFinishedCidr);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CirclPublic::replyFinishedAsn);
}
CirclPublic::~CirclPublic(){
    delete manager;
}

void CirclPublic::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case IP_2_ASN:
            url.setUrl("https://bgpranking-ng.circl.lu/ipasn_history/?ip="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_IP){
        if(args.output_ASN || args.output_CIDR){
            url.setUrl("https://bgpranking-ng.circl.lu/ipasn_history/?ip="+target);
            request.setAttribute(QNetworkRequest::User, IP_2_ASN);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void CirclPublic::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_2_ASN:
        foreach(const QString &date, response.keys()){
            QJsonObject dateObj = response[date].toObject();
            emit resultCIDR(dateObj["prefix"].toString());
            log.resultsCount++;
        }
        break;
    }

    this->end(reply);
}

void CirclPublic::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject response = document.object()["response"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_2_ASN:
        foreach(const QString &date, response.keys()){
            QJsonObject dateObj = response[date].toObject();
            emit resultASN(dateObj["asn"].toString(), "");
            log.resultsCount++;
        }
        break;
    }

    this->end(reply);
}
