#include "Waybackmachine.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*
  different searching ways, produces tons of urls...
  you can get subdomain name by spliting btn http://<your_subdomain>/
  another:
    QUrl url("http://web.archive.org/cdx/search/cdx?url=*."+target+"&output=json&collapse=urlkey");
    https://web.archive.org/cdx/search/cdx?matchType=domain&fl=original&output=txt&collapse=urlkey&url=udsm.ac.tz - text file

    other options:
    &from=2010&to=2018
    &limit=999999
*/

Waybackmachine::Waybackmachine(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Waybackmachine::replyFinished);
}
Waybackmachine::~Waybackmachine(){
    delete manager;
}

void Waybackmachine::start(){
    QNetworkRequest request;
    QUrl url("https://web.archive.org/cdx/search/cdx?matchType=domain&fl=original&output=json&collapse=urlkey&url="+target);
    request.setUrl(url);
    manager->get(request);
}

void Waybackmachine::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        emit errorLog(reply->errorString());
    }
    else
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray mainArray = jsonReply.array();
        mainArray.removeFirst();
        foreach(const QJsonValue &value, mainArray)
            emit scanResults(value.toArray()[0].toString());
    }
    reply->deleteLater();
    emit quitThread();
}
