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

/* also has nameservers and certificates */
/* has some problems... */
ThreatBook::ThreatBook(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "ThreatBook";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &ThreatBook::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &ThreatBook::replyFinishedSubdomain);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &ThreatBook::replyFinishedAsn);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &ThreatBook::replyFinishedIp);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("threatbook").toString();
    Config::generalConfig().endGroup();
}
ThreatBook::~ThreatBook(){
    delete manager;
}

void ThreatBook::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case SUBDOMAINS:
            url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+args->target);
            break;
        case DOMAIN_ADV_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/domain/adv_query?apikey="+m_key+"&resource="+args->target);
            break;
        case DOMAIN_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/domain/query?apikey="+m_key+"&resource="+args->target);
            break;
        case IP_ADV_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/ip/adv_query?apikey="+m_key+"&resource="+args->target);
            break;
        case IP_QUERY:
            url.setUrl("https://api.threatbook.cn/v3/ip/query?apikey="+m_key+"&resource="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+args->target);
            request.setAttribute(QNetworkRequest::User, SUBDOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputIp){
            url.setUrl("https://api.threatbook.cn/v3/domain/query?apikey="+m_key+"&resource="+args->target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_QUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.threatbook.cn/v3/domain/adv_query?apikey="+m_key+"&resource="+args->target);
            request.setAttribute(QNetworkRequest::User, DOMAIN_ADV_QUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        return;
    }

    if(args->inputIp){
        url.setUrl("https://api.threatbook.cn/v3/ip/query?apikey="+m_key+"&resource="+args->target);
        request.setAttribute(QNetworkRequest::User, IP_QUERY);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;

        url.setUrl("https://api.threatbook.cn/v3/ip/adv_query?apikey="+m_key+"&resource="+args->target);
        request.setAttribute(QNetworkRequest::User, IP_ADV_QUERY);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void ThreatBook::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == SUBDOMAINS){
        QJsonArray subdomainList = document.object()["data"].toObject()["sub_domains"].toObject()["data"].toArray();
        foreach(const QJsonValue &value, subdomainList){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
    }

    if(requestType == IP_ADV_QUERY){
        QJsonObject data = document.object()["data"].toObject();
        QJsonObject history_domains = data["history_domains"].toObject();
        QStringList historyDomainsList = history_domains.keys();

        foreach(const QString &historyDomain, historyDomainsList){
            QJsonArray domains = history_domains[historyDomain].toArray();
            foreach(const QJsonValue &value, domains){
                emit subdomain(value.toString());
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

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == IP_QUERY){
        QString asnValue = QString::number(data["asn"].toObject()["number"].toInt());
        QString asnName = data["asn"].toObject()["info"].toString();
        emit asn(asnValue, asnName);
        log.resultsCount++;
    }
    end(reply);
}

void ThreatBook::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == DOMAIN_QUERY){
        QJsonArray cur_ips = data["cur_ips"].toArray();
        foreach(const QJsonValue &value, cur_ips){
            emit ip(value.toObject()["ip"].toString());
            log.resultsCount++;
        }
    }

    if(requestType == DOMAIN_ADV_QUERY){
        QJsonArray history_ips = data["history_ips"].toArray();
        foreach(const QJsonValue &history_ip, history_ips){
            QJsonArray ips = history_ip.toObject()["ips"].toArray();
            foreach(const QJsonValue &value, ips){
                emit ip(value.toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}
