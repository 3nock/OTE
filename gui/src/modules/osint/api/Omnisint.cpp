#include "Omnisint.h"
#include <QJsonDocument>
#include <QJsonArray>

#define ALL 0
#define REVERSE_IP 1
#define SUBDOMAIN 2
#define TLD 3

Omnisint::Omnisint(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Omnisint";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Omnisint::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Omnisint::replyFinishedSubdomain);
}
Omnisint::~Omnisint(){
    delete manager;
}

void Omnisint::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case ALL:
            url.setUrl("https://sonar.omnisint.io/all/"+args->target);
            break;
        case REVERSE_IP:
            url.setUrl("https://sonar.omnisint.io/reverse/"+args->target);
            break;
        case SUBDOMAIN:
            url.setUrl("https://sonar.omnisint.io/subdomains/"+args->target);
            break;
        case TLD:
            url.setUrl("https://sonar.omnisint.io/tlds/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputIp){
        url.setUrl("https://sonar.omnisint.io/reverse/"+args->target);
        request.setAttribute(QNetworkRequest::User, REVERSE_IP);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->outputSubdomain){
        url.setUrl("https://sonar.omnisint.io/all/"+args->target);
        request.setAttribute(QNetworkRequest::User, ALL);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Omnisint::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomains = document.array();

        if(requestType == ALL){
            foreach(const QJsonValue &value, subdomains){
                emit subdomain(value.toString());
                log.resultsCount++;
            }
        }

        if(requestType == REVERSE_IP){
            foreach(const QJsonValue &value, subdomains){
                emit subdomain(value.toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}
