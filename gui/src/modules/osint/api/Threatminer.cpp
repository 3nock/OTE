#include "Threatminer.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define DOMAIN_PASSIVE_DNS 0
#define DOMAIN_QUERY_URI 1
#define DOMAIN_SUBDOMAINS 2
#define DOMAIN_WHOIS 3
#define IP_PASSIVE_DNS 4
#define IP_QUERY_URI 5
#define IP_WHOIS 6
#define SSL_CERTS 7
#define SSL_HOSTS 8


Threatminer::Threatminer(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "ThreatMiner";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedRaw);
}
Threatminer::~Threatminer(){
    delete manager;
}

void Threatminer::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption){
        case DOMAIN_WHOIS:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=1");
            break;
        case DOMAIN_PASSIVE_DNS:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=2");
            break;
        case DOMAIN_SUBDOMAINS:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=5");
            break;
        case DOMAIN_QUERY_URI:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=3");
            break;
        case IP_WHOIS:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=1");
            break;
        case IP_PASSIVE_DNS:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=2");
            break;
        case IP_QUERY_URI:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=3");
            break;
        case SSL_CERTS:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=5");
            break;
        case SSL_HOSTS:
            url.setUrl("https://api.threatminer.org/v2/ssl.php?q="+args->target+"&rt=1");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=5");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Threatminer::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    if(mainObj["status_code"].toString() == "200"){
        QJsonArray results = mainObj["results"].toArray();
        foreach(const QJsonValue &value, results){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
