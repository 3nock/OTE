#include "Threatminer.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Threatminer::Threatminer(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Threatminer::replyFinished);
}
Threatminer::~Threatminer(){
    delete manager;
}

void Threatminer::start(){
    QNetworkRequest request;
    QUrl url("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=5");
    request.setUrl(url);
    manager->get(request);
}

void Threatminer::replyFinished(QNetworkReply *reply){
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
        if(jsonObject["status_code"].toString() == "200")
        {
            QJsonArray results = jsonObject["results"].toArray();
            foreach(const QJsonValue &value, results)
                emit scanResults(value.toString());
        }
        else
        {
            emit errorLog("An Error Ocurred: "+jsonObject["status_code"].toString());
        }
    }
    reply->deleteLater();
    emit quitThread();
}
