#include "GoogleCert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*
 * max pages i have put 6
 */

GoogleCert::GoogleCert(QString _target, QObject *parent):
    AbstractModule (parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &GoogleCert::replyFinished);
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
}

void GoogleCert::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QByteArray replyData = reply->readAll();
        replyData.remove(0, 6);
        //...
        QJsonDocument document = QJsonDocument::fromJson(replyData);
        QJsonArray mainArray = document.array();
        QJsonValue subArray = mainArray[0];
        ///
        /// get subdomanis...
        ///
        QJsonArray subdomainsArray = subArray.toArray()[1].toArray();
        foreach(const QJsonValue &value, subdomainsArray){
            QString hostname = value.toArray()[1].toString();
            /*
            QString certName = value.toArray()[2].toString();
            QString intValue1 = value.toArray()[3].toString();
            QString intValue2 = value.toArray()[4].toString();
            QString shaValue = value.toArray()[5].toString();
            */
            emit scanResults(hostname);
        }
        ///
        /// getting the token...
        ///
        QJsonArray tokenArray = subArray.toArray()[3].toArray();
        QString token = tokenArray[1].toString();
        int currentPage = tokenArray[3].toInt();
        int lastPage = tokenArray[4].toInt();
        ///
        /// send another requests for nextpage...
        ///
        if((lastPage > currentPage) && (currentPage < 6)){
            QNetworkRequest request;
            QUrl url("https://www.google.com/transparencyreport/api/v3/httpsreport/ct/certsearch/page?p="+token);
            request.setUrl(url);
            request.setRawHeader("Connection", "close");
            manager->get(request);
        }
        else {
            reply->deleteLater();
            emit quitThread();
        }
    }
    else
    {
        emit scanResults(reply->errorString());
        //...
        reply->deleteLater();
        emit quitThread();
    }
}
