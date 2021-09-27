#include "ThreatBook.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


Anubis::Anubis(QString _target, QObject *parent):
    AbstractOsintModule(parent)
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
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    manager->get(request);
}

void Anubis::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.array();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
