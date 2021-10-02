#include "Qwant.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*
        Fix the api, now it returns no results:
            https://api.qwant.com/api/search/${keyword}?count=${count}&q=${query}&t=${query}&f=&offset=${offset}&locale=${lang}&uiv=4
*/

Qwant::Qwant(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Qwant::replyFinished);
}
Qwant::~Qwant(){
    delete manager;
}

void Qwant::start(){
    QNetworkRequest request;
    m_offset = 0;
    QUrl url("https://api.qwant.com/api/search/web?count=10&offset="+QString::number(m_offset)+"&q="+target+"&t=web&r=US&device=desktop&safesearch=0&locale=en_US&uiv=4");
    request.setUrl(url);
    manager->get(request);
}

void Qwant::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = document.object();
        if(jsonObject["status"].toString() == QString("success")){
            QJsonObject results = jsonObject["data"].toObject();
            QJsonObject data = results["result"].toObject();
            int resultsCount = data["total"].toInt();
            if(resultsCount > 0){
                ///
                /// getting subdomains...
                ///
                QJsonArray subdomains = data["items"].toArray();
                foreach(const QJsonValue &value, subdomains)
                    emit scanResults(value.toString());
                ///
                /// sending another request for more results...
                ///
                m_offset++;
                QNetworkRequest request;
                QUrl url("https://api.qwant.com/api/search/web?count=10&offset="+QString::number(m_offset)+"&q="+target+"&t=web&r=US&device=desktop&safesearch=0&locale=en_US&uiv=4");
                request.setUrl(url);
                manager->get(request);
                ///
                /// ...
                ///
                reply->deleteLater();
            }
            else{
                reply->deleteLater();
                emit quitThread();
            }
        }
        else{
            reply->deleteLater();
            emit quitThread();
        }
    }
    else
    {
        emit errorLog(reply->errorString());
        reply->deleteLater();
        emit quitThread();
    }
}

