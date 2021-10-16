#include "Urlscan.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*
 * does not produce clean results yet...
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

    QUrl url;
    if(args->raw){
        if(args->option == "domain")
            url.setUrl("https://urlscan.io/api/v1/search/?q=domain:"+args->target);
    }else{
        url.setUrl("https://urlscan.io/api/v1/search/?q=domain:"+args->target);
    }

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
            emit subdomain(page["domain"].toString());
            /*
            emit subdomain(page["server"].toString());
            emit subdomain(page["asn"].toString());
            emit subdomain(page["url"].toString());
            emit subdomain(page["ptr"].toString());
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
