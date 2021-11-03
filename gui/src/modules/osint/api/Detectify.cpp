#include "Detectify.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define SUBDOMAIN 0

Detectify::Detectify(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Detectify";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Detectify::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Detectify::replyFinishedSubdomain);
}
Detectify::~Detectify(){
    delete manager;
}

void Detectify::start(){
    QNetworkRequest request;

    if(args->raw){
        QUrl url("https://jldc.me/Detectify/subdomains/"+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Detectify::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());


    end(reply);
}
