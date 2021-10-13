#include "Bgpview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


Bgpview::Bgpview(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Bgpview::replyFinished);
}
Bgpview::~Bgpview(){
    delete manager;
}

void Bgpview::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "ip")
            url.setUrl("https://api.bgpview.io/ip/"+args->target);
        if(args->option == "ip prefix")
            url.setUrl("https://api.bgpview.io/prefix/"+args->target);
        if(args->option == "asn")
            url.setUrl("https://api.bgpview.io/asn/"+args->target);
        if(args->option == "asn prefixes")
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/prefixes");
        if(args->option == "asn peers")
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/peers");
        if(args->option == "asn upstreams")
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/upstreams");
        if(args->option == "asn downstreams")
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/downstreams");
        if(args->option == "asn ixs")
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/ixs");
        if(args->option == "query")
            url.setUrl("https://api.bgpview.io/search?query_term="+args->target);
    }else{
        url.setUrl("https://api.bgpview.io/search?query_term="+args->target);
    }

    request.setUrl(url);
    manager->get(request);
}

void Bgpview::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.array();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
