#include "Anubis.h"
#include <QJsonDocument>
#include <QJsonArray>


Anubis::Anubis(QString _target, QObject *parent):
    AbstractModule (parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Anubis::replyFinished);
}
Anubis::~Anubis(){
    delete manager;
}

void Anubis::start(){
    QNetworkRequest request;
    QUrl url("https://jldc.me/anubis/subdomains/"+target);
    request.setUrl(url);
    manager->get(request);
}

void Anubis::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        // an error occured...
    }
    else
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.array();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
    }
    reply->deleteLater();
    emit quitThread();
}
