#include "Otx.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Otx::Otx(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Otx::replyFinished);
}
Otx::~Otx(){
    delete manager;
}

void Otx::start(){
    QNetworkRequest request;
    QUrl url("https://otx.alienvault.com/api/v1/indicators/domain/"+args->target+"/passive_dns");
    request.setUrl(url);
    manager->get(request);
}

void Otx::replyFinished(QNetworkReply *reply){
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
        QJsonObject jsonObject = jsonReply.object();
        QJsonArray passive_dns = jsonObject["passive_dns"].toArray();
        foreach(const QJsonValue &value, passive_dns)
            emit scanResults(value["hostname"].toString());
    }
    reply->deleteLater();
    emit quitThread();
}
