#include "ThreatBook.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


ThreatBook::ThreatBook(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ThreatBook::replyFinished);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("threatbook").toString();
    Config::generalConfig().endGroup();
}
ThreatBook::~ThreatBook(){
    delete manager;
}

void ThreatBook::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "subdomains")
            url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+args->target);
        if(args->option == "domain query")
            url.setUrl("https://api.threatbook.cn/v3/domain/query?apikey="+m_key+"&resource="+args->target);
        if(args->option == "ip query")
            url.setUrl("https://api.threatbook.cn/v3/ip/query?apikey="+m_key+"&resource="+args->target);
    }else{
        url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+args->target);
    }

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void ThreatBook::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.object()["data"].toObject()["sub_domains"].toObject()["data"].toArray();
        foreach(const QJsonValue &value, subdomainList)
            emit subdomain(value.toString());
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
