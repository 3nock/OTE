#include "PassiveTotal.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ACCOUNT 0
#define ACCOUNT_CLASSIFICATION 1
#define ACCOUNT_HISTORY 2
#define ACCOUNT_MONITORS 3
#define ACCOUNT_ORGANIZATION 4
#define ACCOUNT_QUOTAS 5
#define ACCOUNT_SOURCES 6
#define ACCOUNT_TEAMSTREAM 7
#define CERTIFICATE 8
#define CERTIFICATE_HISTORY 9
#define CERTIFICATE_KEYWORD 10
#define PASSIVE_DNS 11
#define PASSIVE_DNS_SEARCH 12
#define PASSIVE_DNS_UNIQUE 13
#define WHOIS 14
#define WHOIS_KEYWORD 15


PassiveTotal::PassiveTotal(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_RISKIQ;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &PassiveTotal::replyFinishedRawJson);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &PassiveTotal::replyFinishedIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &PassiveTotal::replyFinishedSubdomain);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &PassiveTotal::replyFinishedSubdomainIp);
    if(args.outputSSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &PassiveTotal::replyFinishedSSL);

    /* getting api-key */
    m_username = APIKEY.value("passivetotal_username").toString();
    m_key = APIKEY.value("passivetotal_key").toString();
}
PassiveTotal::~PassiveTotal(){
    delete manager;
}

void PassiveTotal::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    QString concatenated = m_key+":"+m_username;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", "Basic "+data);

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ACCOUNT:
            url.setUrl("https://api.riskiq.net/pt/v2/account");
            break;
        case ACCOUNT_CLASSIFICATION:
            url.setUrl("https://api.riskiq.net/pt/v2/account/classifications");
            break;
        case ACCOUNT_TEAMSTREAM:
            url.setUrl("https://api.riskiq.net/pt/v2/account/organization/teamstream");
            break;
        case ACCOUNT_SOURCES:
            url.setUrl("https://api.riskiq.net/pt/v2/account/sources");
            break;
        case ACCOUNT_QUOTAS:
            url.setUrl("https://api.riskiq.net/pt/v2/account/quota");
            break;
        case ACCOUNT_ORGANIZATION:
            url.setUrl("https://api.riskiq.net/pt/v2/account/organization");
            break;
        case ACCOUNT_MONITORS:
            url.setUrl("https://api.riskiq.net/pt/v2/account/monitors");
            break;
        case ACCOUNT_HISTORY:
            url.setUrl("https://api.riskiq.net/pt/v2/account/history");
            break;
        case CERTIFICATE:
            url.setUrl("'https://api.riskiq.net/pt/v2/ssl-certificate?query="+target);
            break;
        case CERTIFICATE_HISTORY:
            url.setUrl("https://api.riskiq.net/pt/v2/ssl-certificate/history?query="+target);
            break;
        case CERTIFICATE_KEYWORD:
            url.setUrl("https://api.riskiq.net/pt/v2/ssl-certificate/search/keyword?query="+target);
            break;
        case PASSIVE_DNS:
            url.setUrl("https://api.riskiq.net/pt/v2/dns/passive?query="+target);
            break;
        case PASSIVE_DNS_UNIQUE:
            url.setUrl("https://api.riskiq.net/pt/v2/dns/passive/unique?query="+target);
            break;
        case PASSIVE_DNS_SEARCH:
            url.setUrl("https://api.riskiq.net/pt/v2/dns/search/keyword?query="+target);
            break;
        case WHOIS:
            url.setUrl("https://api.riskiq.net/pt/v2/whois?query=passivetotal.org"+target);
            break;
        case WHOIS_KEYWORD:
            url.setUrl("https://api.riskiq.net/pt/v2/whois/search/keyword?query="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputSubdomainIp || args.outputSubdomain || args.outputIp){
            url.setUrl("https://api.riskiq.net/pt/v2/dns/passive?query="+target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputIp){
        if(args.outputSSL){
            url.setUrl("https://api.riskiq.net/pt/v2/ssl-certificate/history?query="+target);
            request.setAttribute(QNetworkRequest::User, CERTIFICATE_HISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputSSL){
        if(args.outputIp){
            url.setUrl("https://api.riskiq.net/pt/v2/ssl-certificate/history?query="+target);
            request.setAttribute(QNetworkRequest::User, CERTIFICATE_HISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputQueryTerm){
        if(args.outputSubdomain){
            url.setUrl("https://api.riskiq.net/pt/v2/dns/search/keyword?query="+target);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS_SEARCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.outputSSL){
            url.setUrl("https://api.riskiq.net/pt/v2/ssl-certificate/search/keyword?query="+target);
            request.setAttribute(QNetworkRequest::User, CERTIFICATE_KEYWORD);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void PassiveTotal::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_DNS:
        foreach(const QJsonValue &result, results){
            QString value = result.toObject()["value"].toString();
            QString resolve = result.toObject()["resolve"].toString();
            emit resultSubdomainIp(value, resolve);
            log.resultsCount++;
        }
        break;

    case PASSIVE_DNS_SEARCH:
        foreach(const QJsonValue &result, results){
            emit resultSubdomain(result.toObject()["focusPoint"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void PassiveTotal::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_DNS_SEARCH:
        foreach(const QJsonValue &result, results){
            emit resultSubdomain(result.toObject()["focusPoint"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void PassiveTotal::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_DNS:
        foreach(const QJsonValue &result, results){
            emit resultIP(result.toObject()["resolve"].toString());
            log.resultsCount++;
        }
        break;

    case CERTIFICATE_HISTORY:
        foreach(const QJsonValue &result, results){
            foreach(const QJsonValue &ip, result.toObject()["ipAddresses"].toArray()){
                emit resultIP(ip.toString());
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}

void PassiveTotal::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray results = document.object()["results"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case CERTIFICATE_KEYWORD:
        foreach(const QJsonValue &result, results){
            emit resultSSL(result.toObject()["focusPoint"].toString());
            log.resultsCount++;
        }
        break;

    case CERTIFICATE_HISTORY:
        foreach(const QJsonValue &result, results){
            emit resultSSL(result.toObject()["sha1"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
