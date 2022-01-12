#include "GoogleCert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define MAX_PAGE 10

/*
 * for now it goes request after request, later implement concurrent requests...
 * fix and cert output...
 */
GoogleCert::GoogleCert(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "GoogleCert";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &GoogleCert::replyFinishedRawJson);
    if(args.outputSSLCert)
        connect(manager, &s3sNetworkAccessManager::finished, this, &GoogleCert::replyFinishedSSLCert);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &GoogleCert::replyFinishedSubdomain);
}
GoogleCert::~GoogleCert(){
    delete manager;
}

void GoogleCert::start(){
    QNetworkRequest request;
    QUrl url("https://www.google.com/transparencyreport/api/v3/httpsreport/ct/certsearch?include_subdomains=true&domain="+target);
    request.setUrl(url);
    request.setRawHeader("Connection", "close");
    request.setRawHeader("Referer", "https://transparencyreport.google.com/https/certificates");
    manager->get(request);
    activeRequests++;
}

void GoogleCert::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }
    /* removing the first six characters that preceed the json document */
    QByteArray replyData = reply->readAll();
    replyData.remove(0, 6);

    /* get json object that contains the cert data */
    QJsonDocument document = QJsonDocument::fromJson(replyData);
    QJsonValue subArray = document.array()[0];;

    /* obtaining subdomains from current page */
    QJsonArray subdomainsArray = subArray.toArray()[1].toArray();
    foreach(const QJsonValue &value, subdomainsArray){
        QString hostname = value.toArray()[1].toString();
        emit resultSubdomain(hostname);
        log.resultsCount++;
    }

    /* getting token and sending request for next page */
    QJsonArray tokenArray = subArray.toArray()[3].toArray();
    m_getToken(tokenArray);

    end(reply);
}

void GoogleCert::replyFinishedSSLCert(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }
    /* removing the first six characters that preceed the json document */
    QByteArray replyData = reply->readAll();
    replyData.remove(0, 6);

    /* get json object that contains the cert data */
    QJsonDocument document = QJsonDocument::fromJson(replyData);
    QJsonValue subArray = document.array()[0];

    /* obtaining cert fingerprint from current page */
    QJsonArray subdomainsArray = subArray.toArray()[1].toArray();
    foreach(const QJsonValue &value, subdomainsArray){
        QString shaValue = value.toArray()[5].toString();
        shaValue.remove("=");
        emit resultSSL(shaValue);
        log.resultsCount++;
    }

    /* getting token and sending request for next page */
    QJsonArray tokenArray = subArray.toArray()[3].toArray();
    m_getToken(tokenArray);

    end(reply);
}

void GoogleCert::m_getToken(QJsonArray tokenArray){
    QString token = tokenArray[1].toString();
    int currentPage = tokenArray[3].toInt();
    int lastPage = tokenArray[4].toInt();

    /* send another requests for nextpage */
    if((lastPage > currentPage) && (currentPage < MAX_PAGE)){
        QNetworkRequest request;
        QUrl url("https://www.google.com/transparencyreport/api/v3/httpsreport/ct/certsearch/page?p="+token);
        request.setUrl(url);
        request.setRawHeader("Connection", "close");
        manager->get(request);
        activeRequests++;
    }
}
