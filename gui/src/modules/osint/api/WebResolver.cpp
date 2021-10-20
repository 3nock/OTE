#include "WebResolver.h"
#include <QJsonDocument>
#include <QJsonArray>


WebResolver::WebResolver(ScanArgs *args):
    AbstractOsintModule(args)
{
    auto manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &WebResolver::replyFinished);
    //...
    log.moduleName = "WebResolver";
    log.resultsCount = 0;
}
WebResolver::~WebResolver(){
    delete manager;
}

void WebResolver::start(){
    QNetworkRequest request;

    if(args->raw || args->inputDomain){
        QUrl url("https://jldc.me/anubis/subdomains/"+args->target);
        request.setUrl(url);
        manager->get(request);
    }
}

void WebResolver::replyFinished(QNetworkReply *reply){
    log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error())
        this->onError(reply);
    else
    {
        if(args->raw)
            emit rawResults(reply->readAll());
    }
}
