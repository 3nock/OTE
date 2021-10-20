#include "Certspotter.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

/*
 * free for 100 queries an hour - keep track
 */
/*
  TODO:
     filter the stars eg
            *.googlecnapps.cn
            googlecnapps.cn
 */
Certspotter::Certspotter(ScanArgs *args) :
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &Certspotter::replyFinished);
}
Certspotter::~Certspotter(){
    delete manager;
}

void Certspotter::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "cert")
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&expand=dns_names");
    }else{
        url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&expand=dns_names");
    }

    request.setUrl(url);
    manager->get(request);
}

void Certspotter::replyFinished(QNetworkReply *reply){
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
        QJsonArray certsArray = jsonReply.array();
        QJsonObject certObj;
        QJsonArray dns_names;
        foreach(const QJsonValue &value, certsArray)
        {
            certObj = value.toObject();
            dns_names = certObj["dns_names"].toArray();
            foreach(const QJsonValue &item, dns_names)
                emit subdomain(item.toString());
        }
    }
    reply->deleteLater();
    emit quitThread();
}
