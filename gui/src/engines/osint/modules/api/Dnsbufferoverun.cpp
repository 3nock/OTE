#include "Dnsbufferoverun.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Dnsbufferoverun::Dnsbufferoverun(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinished);
}
Dnsbufferoverun::~Dnsbufferoverun(){
    delete manager;
}

void Dnsbufferoverun::start(){
    QNetworkRequest request;
    QUrl url("https://dns.bufferover.run/dns?q="+target);
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
