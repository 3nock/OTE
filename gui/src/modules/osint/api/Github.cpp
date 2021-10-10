#include "Github.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


Github::Github(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Github::replyFinished);
    ///
    /// getting the api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("github").toString();
    Config::generalConfig().endGroup();
}
Github::~Github(){
    delete manager;
}

void Github::start(){
    QNetworkRequest request;
    QUrl url("https://api.github.com/search/code?q="+args->target+"&page=1&per_page=100");
    request.setUrl(url);
    request.setRawHeader("Authorization", "token "+m_key.toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void Github::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        emit scanResults(QString::fromUtf8(reply->readAll()));
        /*
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.array();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
        */
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
