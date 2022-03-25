#include "ThreatBook.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DOMAIN_ADV_QUERY 0
#define DOMAIN_QUERY 1
#define IP_ADV_QUERY 2
#define IP_QUERY 3
#define SUBDOMAINS 4


ThreatBook::ThreatBook(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_THREATBOOK;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ThreatBook::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ThreatBook::replyFinishedSubdomain);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ThreatBook::replyFinishedAsn);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ThreatBook::replyFinishedIp);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_THREATBOOK).toString();
}
ThreatBook::~ThreatBook(){
    delete manager;
}

void ThreatBook::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case SUBDOMAINS:
            url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+target);
            break;
        case DOMAIN_ADV_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/domain/adv_query?apikey="+m_key+"&resource="+target);
            break;
        case DOMAIN_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/domain/query?apikey="+m_key+"&resource="+target);
            break;
        case IP_ADV_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/ip/adv_query?apikey="+m_key+"&resource="+target);
            break;
        case IP_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/ip/query?apikey="+m_key+"&resource="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+target);
            request.setAttribute(QNetworkRequest::User, SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.output_IP){
            url.setUrl("https://api.threatbook.cn/v3/domain/query?apikey="+m_key+"&resource="+target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_QUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            /*
            url.setUrl("https://api.threatbook.cn/v3/domain/adv_query?apikey="+m_key+"&resource="+target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_ADV_QUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            */
        }
    }

    if(args.input_IP){
        url.setUrl("https://api.threatbook.cn/v3/ip/query?apikey="+m_key+"&resource="+target);
        request.setAttribute(QNetworkRequest::User, IP_QUERY);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;

        /*
        url.setUrl("https://api.threatbook.cn/v3/ip/adv_query?apikey="+m_key+"&resource="+target);
        request.setAttribute(QNetworkRequest::User, IP_ADV_QUERY);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        */
    }
}

void ThreatBook::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case SUBDOMAINS:
    {
        QJsonArray subdomainList = document.object()["data"].toObject()["sub_domains"].toObject()["data"].toArray();
        foreach(const QJsonValue &value, subdomainList){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
    }
        break;

    case IP_ADV_QUERY:
        QJsonObject data = document.object()["data"].toObject();
        QJsonObject history_domains = data["history_domains"].toObject();
        QStringList historyDomainsList = history_domains.keys();

        foreach(const QString &historyDomain, historyDomainsList){
            QJsonArray domains = history_domains[historyDomain].toArray();
            foreach(const QJsonValue &value, domains){
                emit resultSubdomain(value.toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void ThreatBook::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_QUERY:
        QString asnValue = QString::number(data["asn"].toObject()["number"].toInt());
        QString asnName = data["asn"].toObject()["info"].toString();
        emit resultASN(asnValue, asnName);
        log.resultsCount++;
    }

    end(reply);
}

void ThreatBook::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt()) {
    case DOMAIN_QUERY:
    {
        QJsonArray cur_ips = data["cur_ips"].toArray();
        foreach(const QJsonValue &value, cur_ips){
            emit resultIP(value.toObject()["ip"].toString());
            log.resultsCount++;
        }
    }
        break;

    case DOMAIN_ADV_QUERY:
        QJsonArray history_ips = data["history_ips"].toArray();
        foreach(const QJsonValue &history_ip, history_ips){
            QJsonArray ips = history_ip.toObject()["ips"].toArray();
            foreach(const QJsonValue &value, ips){
                emit resultIP(value.toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}
