#include "Spyse.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Spyse::Spyse(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Spyse::replyFinished);
}
Spyse::~Spyse(){
    delete manager;
}

void Spyse::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "Domain")
            url.setUrl("https://api.spyse.com/v4/data/domain/"+args->target);
        if(args->option == "IPv4 Host")
            url.setUrl("https://api.spyse.com/v4/data/ip/"+args->target);
        if(args->option == "SSL/TLS Certificate")
            url.setUrl("https://api.spyse.com/v4/data/certificate/"+args->target);
        if(args->option == "AS")
            url.setUrl("https://api.spyse.com/v4/data/as/"+args->target);
        if(args->option == "CVE")
            url.setUrl("https://api.spyse.com/v4/data/cve/"+args->target);
        if(args->option == "Emails")
            url.setUrl("https://api.spyse.com/v4/data/email/"+args->target);
        if(args->option == "DNS History")
            url.setUrl("https://api.spyse.com/v4/data/history/dns/ANY/"+args->target);
    }else{
        url.setUrl("https://api.spyse.com/v3/data/domain/subdomain?limit=100&domain="+args->target);
    }

    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Authorization", "Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0");
    request.setRawHeader("Content-Type", "application/json");
    request.setUrl(url);
    manager->get(request);
}

void Spyse::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        emit errorLog(reply->errorString());
    }
    else
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject dataObject = jsonReply.object()["data"].toObject();
        QJsonArray items = dataObject["items"].toArray();
        foreach(const QJsonValue &value, items)
            emit subdomain(value["name"].toString());

        /*
                      Emails
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject dataObject = jsonReply.object()["data"].toObject();
        QJsonArray items = dataObject["items"].toArray();
        foreach(const QJsonValue &value, items)
            emit subdomain(value["email"].toString());

         */
    }
    reply->deleteLater();
    emit quitThread();
}
