#include "Sublist3r.h"
#include <QJsonDocument>
#include <QJsonArray>


Sublist3r::Sublist3r(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_SUBLIST3R;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Sublist3r::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Sublist3r::replyFinishedSubdomain);
}
Sublist3r::~Sublist3r(){
    delete manager;
}

void Sublist3r::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        url.setUrl("https://api.sublist3r.com/search.php?domain="+target);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        url.setUrl("https://api.sublist3r.com/search.php?domain="+target);
        request.setUrl(url);
        manager->get(request);
    }
}

void Sublist3r::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray passive_dns = document.array();

    foreach(const QJsonValue &value, passive_dns){
        emit resultSubdomain(value.toString());
        log.resultsCount++;
    }
    this->end(reply);
}
