#include "Sublist3r.h"
#include <QJsonDocument>
#include <QJsonArray>


Sublist3r::Sublist3r(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Sublist3r::replyFinished);
}
Sublist3r::~Sublist3r(){
    delete manager;
}

void Sublist3r::start(){
    QNetworkRequest request;
    QUrl url("https://api.sublist3r.com/search.php?domain="+target);
    request.setUrl(url);
    manager->get(request);
}

void Sublist3r::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        // an error occured...
    }
    else
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray passive_dns = jsonReply.array();
        foreach(const QJsonValue &value, passive_dns)
            emit scanResults(value.toString());
    }
    reply->deleteLater();
    emit quitThread();
}
