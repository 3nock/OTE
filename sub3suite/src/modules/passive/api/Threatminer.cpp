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
Threatminer::Threatminer(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_THREATMINER;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatminer::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatminer::replyFinishedSubdomain);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatminer::replyFinishedEmail);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatminer::replyFinishedIp);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatminer::replyFinishedAsn);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatminer::replyFinishedUrl);
    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Threatminer::replyFinishedSSL);
}
Threatminer::~Threatminer(){
    delete manager;
}

void Threatminer::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id){
        case DOMAIN_WHOIS:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=1");
            break;
        case DOMAIN_PASSIVE_DNS:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=2");
            break;
        case DOMAIN_SUBDOMAINS:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=5");
            break;
        case DOMAIN_QUERY_URI:
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=3");
            break;
        case IP_WHOIS:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+target+"&rt=1");
            break;
        case IP_PASSIVE_DNS:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+target+"&rt=2");
            break;
        case IP_QUERY_URI:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+target+"&rt=3");
            break;
        case IP_SSL_CERTS:
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+target+"&rt=5");
            break;
        case SSL_HOSTS:
            url.setUrl("https://api.threatminer.org/v2/ssl.php?q="+target+"&rt=1");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /* for domain name target */
    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=5");
            request.setAttribute(QNetworkRequest::User, DOMAIN_SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.output_Email){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=1");
            request.setAttribute(QNetworkRequest::User, DOMAIN_WHOIS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.output_IP){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=2");
            request.setAttribute(QNetworkRequest::User, DOMAIN_PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.output_URL){
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+target+"&rt=3");
            request.setAttribute(QNetworkRequest::User, DOMAIN_QUERY_URI);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    /* For ip-address target */
    if(args.input_IP){
        if(args.output_ASN){
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+target+"&rt=1");
            request.setAttribute(QNetworkRequest::User, IP_WHOIS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.output_Hostname){
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+target+"&rt=2");
            request.setAttribute(QNetworkRequest::User, IP_PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.output_SSL){
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+target+"&rt=5");
            request.setAttribute(QNetworkRequest::User, IP_SSL_CERTS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    /* for ssl-cert hash target */
    if(args.input_SSL){
        if(args.output_IP){
            url.setUrl("https://api.threatminer.org/v2/ssl.php?q="+target+"&rt=1");
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_SUBDOMAINS:
        foreach(const QJsonValue &result, results){
            QString hostname = result.toString();
            emit resultSubdomain(hostname);
            log.resultsCount++;
        }
        break;

    case IP_PASSIVE_DNS:
        foreach(const QJsonValue &result, results){
            QString hostname = result.toObject()["domain"].toString();
            emit resultSubdomain(hostname);
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_PASSIVE_DNS:
        foreach(const QJsonValue &result, results){
            QString address = result.toObject()["ip"].toString();
            emit resultIP(address);
            log.resultsCount++;
        }
        break;

    case SSL_HOSTS:
        foreach(const QJsonValue &result, results){
            QString address = result.toString();
            emit resultIP(address);
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAIN_WHOIS:
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_WHOIS:
        foreach(const QJsonValue &result, results){
            QString asnValue = result.toObject()["asn"].toString();
            QString asnName = result.toObject()["asn_name"].toString();
            emit resultASN(asnValue, asnName);
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case DOMAIN_QUERY_URI:
    case IP_QUERY_URI:
        foreach(const QJsonValue &result, results){
            QString uri = result.toObject()["uri"].toString();
            emit resultURL(uri);
            log.resultsCount++;
        }
    }

    end(reply);
}

void Threatminer::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case IP_SSL_CERTS:
        foreach(const QJsonValue &result, results){
            QString hash = result.toString();
            emit resultSSL(hash);
            log.resultsCount++;
        }
    }

    end(reply);
}
