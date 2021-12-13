#include "Spyse.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define AS 0
#define CVE 1
#define EMAILS 2
#define DNS_HISTORY 3
#define DOMAINS 4
#define IPV4 5
#define IPV6 6
#define SSL_CERT 7

/* also returns ptr records for an ip in IPV4 */
/* returns a good ssl cert summary */
/* have not implemented bulk-seacrh & dns history */
/* 100 queries for a free account */
Spyse::Spyse(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Spyse";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Spyse::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Spyse::replyFinishedSubdomain);
    if(args.outputEmail)
        connect(manager, &NetworkAccessManager::finished, this, &Spyse::replyFinishedEmail);
    if(args.outputAsn)
        connect(manager, &NetworkAccessManager::finished, this, &Spyse::replyFinishedAsn);
    if(args.outputSSLCert)
        connect(manager, &NetworkAccessManager::finished, this, &Spyse::replyFinishedSSLCert);
    if(args.outputUrl)
        connect(manager, &NetworkAccessManager::finished, this, &Spyse::replyFinishedUrl);
    if(args.outputIp)
        connect(manager, &NetworkAccessManager::finished, this, &Spyse::replyFinishedIp);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("spyse").toString();
    Config::generalConfig().endGroup();
}
Spyse::~Spyse(){
    delete manager;
}

void Spyse::start(){
    QNetworkRequest request;
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "Bearer "+m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case DOMAINS:
            url.setUrl("https://api.spyse.com/v4/data/domain/"+target);
            break;
        case IPV4:
            url.setUrl("https://api.spyse.com/v4/data/ip/"+target);
            break;
        case SSL_CERT:
            url.setUrl("https://api.spyse.com/v4/data/certificate/"+target);
            break;
        case AS:
            url.setUrl("https://api.spyse.com/v4/data/as/"+target);
            break;
        case CVE:
            url.setUrl("https://api.spyse.com/v4/data/cve/"+target);
            break;
        case EMAILS:
            url.setUrl("https://api.spyse.com/v4/data/email/"+target);
            break;
        case DNS_HISTORY:
            url.setUrl("https://api.spyse.com/v4/data/history/dns/ANY/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        url.setUrl("https://api.spyse.com/v3/data/domain/subdomain?limit=100&domain="+target);
        request.setAttribute(QNetworkRequest::User, DOMAINS);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        url.setUrl("https://api.spyse.com/v4/data/ip/"+target);
        request.setAttribute(QNetworkRequest::User, IPV4);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputSSLCert){
        url.setUrl("https://api.spyse.com/v4/data/certificate/"+target);
        request.setAttribute(QNetworkRequest::User, SSL_CERT);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputAsn){
        url.setUrl("https://api.spyse.com/v4/data/as/"+target);
        request.setAttribute(QNetworkRequest::User, AS);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputEmail){
        url.setUrl("https://api.spyse.com/v4/data/email/"+target);
        request.setAttribute(QNetworkRequest::User, EMAILS);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Spyse::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    if(QUERY_TYPE == DOMAINS){
        foreach(const QJsonValue &item, items){
            QJsonArray emails = item.toObject()["http_extract"].toObject()["emails"].toArray();
            foreach(const QJsonValue &value, emails){
                emit email(value.toString());
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == SSL_CERT){
        foreach(const QJsonValue &item, items){
            QJsonArray email_address = item.toObject()["issuer"].toObject()["email_address"].toArray();
            foreach(const QJsonValue &value, email_address){
                emit email(value.toString());
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == EMAILS){
        foreach(const QJsonValue &item, items){
            QString email_address = item.toObject()["email"].toString();
            emit email(email_address);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Spyse::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    if(QUERY_TYPE == DOMAINS){
        foreach(const QJsonValue &item, items){
            /* from hosts_enrichment */
            QJsonArray hosts_enrichment = item.toObject()["hosts_enrichment"].toArray();
            foreach(const QJsonValue &value, hosts_enrichment){
                emit ip(value.toObject()["ip"].toString());
                log.resultsCount++;
            }
            /* from dns_records */
            QJsonObject dns_records = item.toObject()["dns_records"].toObject();
            QJsonArray A = dns_records["A"].toArray();
            foreach(const QJsonValue &value, A){
                emit ipA(value.toString());
                log.resultsCount++;
            }
            QJsonArray AAAA = dns_records["A"].toArray();
            foreach(const QJsonValue &value, AAAA){
                emit ipAAAA(value.toString());
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == AS){
        foreach(const QJsonValue &item, items){
            QJsonArray ipv4_prefixes = item["ipv4_prefixes"].toArray();
            foreach(const QJsonValue &value, ipv4_prefixes){
                emit ipA(value.toObject()["cidr"].toString());
                log.resultsCount++;
            }
            QJsonArray ipv6_prefixes = item["ipv6_prefixes"].toArray();
            foreach(const QJsonValue &value, ipv6_prefixes){
                emit ipAAAA(value.toObject()["cidr"].toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void Spyse::replyFinishedSSLCert(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    if(QUERY_TYPE == DOMAINS){
        foreach(const QJsonValue &item, items){
            QJsonObject cert_summary = item.toObject()["cert_summary"].toObject();
            QString fingerprint = cert_summary["fingerprint_sha256"].toString();
            emit sslCert(fingerprint);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Spyse::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    if(QUERY_TYPE == DOMAINS){
        foreach(const QJsonValue &item, items){
            QJsonArray hosts_enrichment = item.toObject()["hosts_enrichment"].toArray();
            foreach(const QJsonValue &value, hosts_enrichment){
                QString as_num = QString::number(value.toObject()["as_num"].toInt());
                QString as_org = value.toObject()["as_org"].toString();
                emit asn(as_num, as_org);
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == AS){
        foreach(const QJsonValue &item, items){
            QString as_num = QString::number(item.toObject()["as_num"].toInt());
            QString as_org = item.toObject()["as_org"].toString();

            emit asn(as_num, as_org);
            log.resultsCount++;
        }
    }

    if(QUERY_TYPE == IPV4){
        foreach(const QJsonValue &item, items){
            QJsonObject isp_info = item.toObject()["isp_info"].toObject();

            QString as_num = QString::number(isp_info["as_num"].toInt());
            QString as_org = isp_info["as_org"].toString();

            emit asn(as_num, as_org);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Spyse::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    if(QUERY_TYPE == DOMAINS){
        foreach(const QJsonValue &item, items){
            QJsonArray urls = item.toObject()["http_extract"].toObject()["links"].toArray();
            foreach(const QJsonValue &value, urls){
                emit url(value.toObject()["url"].toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void Spyse::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    if(QUERY_TYPE == DOMAINS){
        foreach(const QJsonValue &item, items)
        {
            emit subdomain(item["name"].toString());
            log.resultsCount++;

            /* from dns_records */
            QJsonObject dns_records = item.toObject()["dns_records"].toObject();
            QJsonArray ns = dns_records["NS"].toArray();
            foreach(const QJsonValue &value, ns){
                emit NS(value.toString());
                log.resultsCount++;
            }
            QJsonArray mx = dns_records["MX"].toArray();
            foreach(const QJsonValue &value, mx){
                emit MX(value.toString());
                log.resultsCount++;
            }
            QJsonArray cname = dns_records["CNAME"].toArray();
            foreach(const QJsonValue &value, cname){
                emit CNAME(value.toString());
                log.resultsCount++;
            }
            QJsonArray txt = dns_records["TXT"].toArray();
            foreach(const QJsonValue &value, txt){
                emit TXT(value.toString());
                log.resultsCount++;
            }
        }
    }
    end(reply);
}
