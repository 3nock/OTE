#include "Urlscan.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*
 * does not produce clean results yet...
 *
{
  "results": [
    {
      "indexedAt": "2021-09-21T18:05:39.615Z",
      "task": {
        "visibility": "public",
        "method": "automatic",
        "domain": "www.smtpauth.diamart.sk",
        "time": "2021-09-21T18:05:31.619Z",
        "source": "certstream-suspicious",
        "uuid": "c884f5eb-dc4a-416e-95a0-18905f3dcb88",
        "url": "https://www.smtpauth.diamart.sk"
      },
      "stats": {
        "uniqIPs": 8,
        "consoleMsgs": 0,
        "uniqCountries": 3,
        "dataLength": 7797947,
        "encodedDataLength": 6329650,
        "requests": 39
      },
      "page": {
        "country": "CZ",
        "server": "nginx",
        "domain": "diamart.webnode.cz",
        "ip": "85.132.152.107",
        "mimeType": "text/html",
        "asnname": "FASTER-AS, CZ",
        "asn": "AS24641",
        "url": "https://diamart.webnode.cz/",
        "ptr": "web-1030.webnode.com",
        "status": "200"
      },
      "_id": "c884f5eb-dc4a-416e-95a0-18905f3dcb88",
      "sort": [
        1632247531619,
        "c884f5eb-dc4a-416e-95a0-18905f3dcb88"
      ],
      "result": "https://urlscan.io/api/v1/result/c884f5eb-dc4a-416e-95a0-18905f3dcb88/",
      "screenshot": "https://urlscan.io/screenshots/c884f5eb-dc4a-416e-95a0-18905f3dcb88.png"
    }
    ]
}
*/

Urlscan::Urlscan(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Urlscan::replyFinished);
}
Urlscan::~Urlscan(){
    delete manager;
}

void Urlscan::start(){
    QNetworkRequest request;
    QUrl url("https://urlscan.io/api/v1/search/?q=domain:"+args->target);
    request.setUrl(url);
    manager->get(request);
}

void Urlscan::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        QJsonArray resultsArray = jsonObject["results"].toArray();
        foreach(const QJsonValue &value, resultsArray){
            QJsonObject page = value["page"].toObject();
            emit scanResults(page["domain"].toString());
            /*
            emit scanResults(page["server"].toString());
            emit scanResults(page["asn"].toString());
            emit scanResults(page["url"].toString());
            emit scanResults(page["ptr"].toString());
            */
        }
    }
    else
    {
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
