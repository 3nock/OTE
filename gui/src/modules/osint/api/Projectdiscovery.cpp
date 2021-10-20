#include "Projectdiscovery.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Projectdiscovery::Projectdiscovery(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &Projectdiscovery::replyFinished);
}
Projectdiscovery::~Projectdiscovery(){
    delete manager;
}

void Projectdiscovery::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "subdomains")
            url.setUrl("https://dns.projectdiscovery.io/dns/"+args->target+"/subdomains");
    }else{
        url.setUrl("https://dns.projectdiscovery.io/dns/"+args->target+"/subdomains");
    }

    request.setUrl(url);
    request.setRawHeader("Authorization", "key");
    manager->get(request);
}

void Projectdiscovery::replyFinished(QNetworkReply *reply){
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
        QString error = jsonObject["error"].toString();
        if(error.isNull() || error.isEmpty()){
            QJsonArray subdomainList = jsonObject["subdomains"].toArray();
            foreach(const QJsonValue &value, subdomainList)
                emit subdomain(value.toString());
        }
        else{
            emit subdomain(error);
        }
    }
    else
    {
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
