#include "Dnsbufferoverun.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Dnsbufferoverun::Dnsbufferoverun(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &Dnsbufferoverun::replyFinished);
    //...
    log.moduleName = "DnsBufferoverRun";
    log.resultsCount = 0;
}
Dnsbufferoverun::~Dnsbufferoverun(){
    delete manager;
}

void Dnsbufferoverun::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        url.setUrl("https://dns.bufferover.run/dns?q="+args->target);
    }
    if(args->inputDomain){
        url.setUrl("https://dns.bufferover.run/dns?q="+args->target);
    }
    request.setUrl(url);

    manager->get(request);
    activeRequests++;
}

void Dnsbufferoverun::replyFinished(QNetworkReply *reply){
    log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error())
        this->onError(reply);
    else
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            goto END;
        }

        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();

        if(args->outputSubdomainIp){
            QJsonArray FDNS_A = jsonObject["FDNS_A"].toArray();
            foreach(const QJsonValue &value, FDNS_A){
                QStringList subdomainAndIp = value.toString().split(","); // ip-address,subdomain
                emit subdomainIp(subdomainAndIp[1], subdomainAndIp[0]);
                log.resultsCount++;
            }
            QJsonArray RDNS = jsonObject["RDNS"].toArray();
            foreach(const QJsonValue &value, RDNS){
                QStringList subdomainAndIp = value.toString().split(","); // ip-address,subdomain
                emit subdomainIp(subdomainAndIp[1], subdomainAndIp[0]);
                log.resultsCount++;
            }
        }

        if(args->outputSubdomain){
            QJsonArray FDNS_A = jsonObject["FDNS_A"].toArray();
            foreach(const QJsonValue &value, FDNS_A){
                emit subdomain(value.toString().split(",")[1]); // subdomain
                log.resultsCount++;
            }
            QJsonArray RDNS = jsonObject["RDNS"].toArray();
            foreach(const QJsonValue &value, RDNS){
                emit subdomain(value.toString().split(",")[1]); // subdomain
                log.resultsCount++;
            }
        }

        if(args->outputIp){
            QJsonArray FDNS_A = jsonObject["FDNS_A"].toArray();
            foreach(const QJsonValue &value, FDNS_A){
                emit ip(value.toString().split(",")[0]); // ip-address
                log.resultsCount++;
            }
            QJsonArray RDNS = jsonObject["RDNS"].toArray();
            foreach(const QJsonValue &value, RDNS){
                emit ip(value.toString().split(",")[0]); // ip-address
                log.resultsCount++;
            }
        }
    }
END:
    reply->deleteLater();
    activeRequests--;
    if(activeRequests == 0){
        //emit infoLog(log);
        emit quitThread();
    }
}
