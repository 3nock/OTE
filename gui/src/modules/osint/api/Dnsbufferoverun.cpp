#include "Dnsbufferoverun.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Dnsbufferoverun::Dnsbufferoverun(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinished);
}
Dnsbufferoverun::~Dnsbufferoverun(){
    delete manager;
}

void Dnsbufferoverun::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        url.setUrl("https://dns.bufferover.run/dns?q="+args->target);
    }else{
        url.setUrl("https://dns.bufferover.run/dns?q="+args->target);
    }

    request.setUrl(url);
    manager->get(request);
}

void Dnsbufferoverun::replyFinished(QNetworkReply *reply){
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

        QJsonArray FDNS_A = jsonObject["FDNS_A"].toArray();
        foreach(const QJsonValue &value, FDNS_A)
            emit scanResults(value.toString());

        QJsonArray RDNS = jsonObject["RDNS"].toArray();
        foreach(const QJsonValue &value, RDNS)
            emit scanResults(value.toString());
    }
    reply->deleteLater();
    emit quitThread();
}
