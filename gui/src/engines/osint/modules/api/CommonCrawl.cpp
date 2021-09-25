#include "CommonCrawl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * provides urls...
 * uses archives of list from different times...
 * returns ndjson format...
 *  try:
 *      Content-Type: application/json
 */

CommonCrawl::CommonCrawl(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &CommonCrawl::replyFinished);
}
CommonCrawl::~CommonCrawl(){
    delete manager;
}

void CommonCrawl::start(){
    QNetworkRequest request;
    QUrl url("https://index.commoncrawl.org/collinfo.json");
    request.setUrl(url);
    manager->get(request);
}

void CommonCrawl::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(getArchive){
            getArchive = false;
            QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
            QJsonArray subdomainList = jsonReply.array();
            foreach(const QJsonValue &value, subdomainList){
                QJsonObject aaa = value.toObject();
                urlList.append(aaa["cdx-api"].toString());
            }
            ///
            /// send request to get the subdomains,
            /// just use the first on the list for now...
            ///
            QUrl url(urlList.at(0)+"?url=*."+target+"&output=json&fl=url");
            QNetworkRequest request;
            request.setUrl(url);
            manager->get(request);
            ///
            /// return...
            ///
            reply->deleteLater();
            return;
        }
        else{
            QString replyList = QString::fromUtf8(reply->readAll());
            QStringList urlList = replyList.split("\n");
            foreach(const QString &value, urlList){
                    QString url = value;
                    url.chop(2);
                    emit scanResults(url.remove(0, 9));
            }
        }
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
