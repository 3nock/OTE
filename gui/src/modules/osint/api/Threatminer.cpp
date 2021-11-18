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
#define IP_SSL_CERTS 6
#define IP_WHOIS 7
#define SSL_HOSTS 8

/*  rate limit is set to 10 queries per minute */
/* email reverse-whois */
Threatminer::Threatminer(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "ThreatMiner";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedSubdomain);
    if(args->outputEmail)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedEmail);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedAsn);
    if(args->outputUrl)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedUrl);
    if(args->outputSSLCert)
        connect(manager, &MyNetworkAccessManager::finished, this, &Threatminer::replyFinishedSSLCert);
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
        case IP_SSL_CERTS:
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

    /* for domain name target */
    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=5");
            request.setAttribute(QNetworkRequest::User, DOMAIN_SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputEmail){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=1");
            request.setAttribute(QNetworkRequest::User, DOMAIN_WHOIS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputIp){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=2");
            request.setAttribute(QNetworkRequest::User, DOMAIN_PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputUrl){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=3");
            request.setAttribute(QNetworkRequest::User, DOMAIN_QUERY_URI);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    /* For ip-address target */
    if(args->inputIp){
        if(args->outputAsn){
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=1");
            request.setAttribute(QNetworkRequest::User, IP_WHOIS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputSubdomain){
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=2");
            request.setAttribute(QNetworkRequest::User, IP_PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputSSLCert){
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=5");
            request.setAttribute(QNetworkRequest::User, IP_SSL_CERTS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    /* for ssl-cert hash target */
    if(args->inputSSLCert){
        if(args->outputIp){
            url.setUrl("https://api.threatminer.org/v2/ssl.php?q="+args->target+"&rt=1");
            request.setAttribute(QNetworkRequest::User, SSL_HOSTS);
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == DOMAIN_SUBDOMAINS){
        foreach(const QJsonValue &result, results){
            QString hostname = result.toString();
            emit subdomain(hostname);
            log.resultsCount++;
        }
    }

    if(requestType == IP_PASSIVE_DNS){
        foreach(const QJsonValue &result, results){
            QString hostname = result.toObject()["domain"].toString();
            emit subdomain(hostname);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Threatminer::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == DOMAIN_PASSIVE_DNS){
        foreach(const QJsonValue &result, results){
            QString address = result.toObject()["ip"].toString();
            emit ip(address);
            log.resultsCount++;
        }
    }

    if(requestType == SSL_HOSTS){
        foreach(const QJsonValue &result, results){
            QString address = result.toString();
            emit ip(address);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Threatminer::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == DOMAIN_WHOIS){
        foreach(const QJsonValue &result, results){
            QJsonObject emails = result.toObject()["whois"].toObject()["emails"].toObject();
            QStringList keys = emails.keys();

            foreach(const QString &key, keys){
                QString email = emails[key].toString();
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Threatminer::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == IP_WHOIS){
        foreach(const QJsonValue &result, results){
            QString asnValue = result.toObject()["asn"].toString();
            QString asnName = result.toObject()["asn_name"].toString();
            emit asn(asnValue, asnName);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Threatminer::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == DOMAIN_QUERY_URI || requestType == IP_QUERY_URI){
        foreach(const QJsonValue &result, results){
            QString uri = result.toObject()["uri"].toString();
            emit url(uri);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Threatminer::replyFinishedSSLCert(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    if(requestType == IP_SSL_CERTS){
        foreach(const QJsonValue &result, results){
            QString hash = result.toString();
            emit sslCert(hash);
            log.resultsCount++;
        }
    }
    end(reply);
}
