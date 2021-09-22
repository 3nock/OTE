#include "Threatcrowd.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Threatcrowd::Threatcrowd(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Threatcrowd::replyFinished);
}
Threatcrowd::~Threatcrowd(){
    delete manager;
}

void Threatcrowd::start(){
    QNetworkRequest request;
    QUrl url("https://www.threatcrowd.org/searchApi/v2/domain/report/?domain="+target);
    request.setUrl(url);
    manager->get(request);
}

void Threatcrowd::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        // an error occured...
    }
    else
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        if(jsonObject["response_code"].toString() == "1")
        {
            /*
                            FOR IP-ADDRESS
            QJsonArray resolutions = jsonObject["resolutions"].toArray();
            foreach(const QJsonValue &value, resolutions)
                emit scanResults(value["ip_address"].toString());

                              FOR EMAILS
            QJsonArray emails = jsonObject["emails"].toArray();
            foreach(const QJsonValue &value, emails)
                emit scanResults(value.toString());
            */

            QJsonArray subdomains = jsonObject["subdomains"].toArray();
            foreach(const QJsonValue &value, subdomains)
                emit scanResults(value.toString());
        }
    }
    reply->deleteLater();
    emit quitThread();
}
