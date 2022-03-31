#include "Spyse.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define AS 0
#define CVE 1
#define DNS_HISTORY 2
#define DOMAINS 3
#define EMAILS 4
#define IPV4 5
#define IPV6 6
#define SSL_CERT 7

/*
 * 100 queries for a free account
 */
Spyse::Spyse(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_SPYSE;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Spyse::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Spyse::replyFinishedSubdomain);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Spyse::replyFinishedEmail);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Spyse::replyFinishedAsn);
    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Spyse::replyFinishedSSL);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Spyse::replyFinishedUrl);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Spyse::replyFinishedIp);

    /* getting api-key */
    m_key = APIKEY.value(OSINT_MODULE_SPYSE).toString();
}
Spyse::~Spyse(){
    delete manager;
}

void Spyse::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "Bearer "+m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
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
        return;
    }

    if(args.input_Domain){
        url.setUrl("https://api.spyse.com/v3/data/domain/subdomain?limit=100&domain="+target);
        request.setAttribute(QNetworkRequest::User, DOMAINS);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_IP){
        url.setUrl("https://api.spyse.com/v4/data/ip/"+target);
        request.setAttribute(QNetworkRequest::User, IPV4);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_SSL){
        url.setUrl("https://api.spyse.com/v4/data/certificate/"+target);
        request.setAttribute(QNetworkRequest::User, SSL_CERT);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_ASN){
        url.setUrl("https://api.spyse.com/v4/data/as/"+target);
        request.setAttribute(QNetworkRequest::User, AS);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Email){
        url.setUrl("https://api.spyse.com/v4/data/email/"+target);
        request.setAttribute(QNetworkRequest::User, EMAILS);
        request.setUrl(url);
        manager->get(request);
    }
}

void Spyse::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS:
        foreach(const QJsonValue &item, items){
            QJsonArray emails = item.toObject()["http_extract"].toObject()["emails"].toArray();
            foreach(const QJsonValue &value, emails){
                emit resultEmail(value.toString());
                log.resultsCount++;
            }
        }
        break;

    case SSL_CERT:
        foreach(const QJsonValue &item, items){
            QJsonArray email_address = item.toObject()["issuer"].toObject()["email_address"].toArray();
            foreach(const QJsonValue &value, email_address){
                emit resultEmail(value.toString());
                log.resultsCount++;
            }
        }
        break;

    case EMAILS:
        foreach(const QJsonValue &item, items){
            QString email_address = item.toObject()["email"].toString();
            emit resultEmail(email_address);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Spyse::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS:
        foreach(const QJsonValue &item, items){
            /* from hosts_enrichment */
            QJsonArray hosts_enrichment = item.toObject()["hosts_enrichment"].toArray();
            foreach(const QJsonValue &value, hosts_enrichment){
                emit resultIP(value.toObject()["ip"].toString());
                log.resultsCount++;
            }
            /* from dns_records */
            QJsonObject dns_records = item.toObject()["dns_records"].toObject();
            QJsonArray A = dns_records["A"].toArray();
            foreach(const QJsonValue &value, A){
                emit resultA(value.toString());
                log.resultsCount++;
            }
            QJsonArray AAAA = dns_records["A"].toArray();
            foreach(const QJsonValue &value, AAAA){
                emit resultAAAA(value.toString());
                log.resultsCount++;
            }
        }
        break;

    case AS:
        foreach(const QJsonValue &item, items){
            QJsonArray ipv4_prefixes = item["ipv4_prefixes"].toArray();
            foreach(const QJsonValue &value, ipv4_prefixes){
                emit resultA(value.toObject()["cidr"].toString());
                log.resultsCount++;
            }
            QJsonArray ipv6_prefixes = item["ipv6_prefixes"].toArray();
            foreach(const QJsonValue &value, ipv6_prefixes){
                emit resultAAAA(value.toObject()["cidr"].toString());
                log.resultsCount++;
            }
        }
    }

    this->end(reply);
}

void Spyse::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS:
        foreach(const QJsonValue &item, items){
            QJsonObject cert_summary = item.toObject()["cert_summary"].toObject();
            QString fingerprint = cert_summary["fingerprint_sha256"].toString();
            emit resultSSL(fingerprint);
            log.resultsCount++;
        }
    }
    this->end(reply);
}

void Spyse::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS:
        foreach(const QJsonValue &item, items){
            QJsonArray hosts_enrichment = item.toObject()["hosts_enrichment"].toArray();
            foreach(const QJsonValue &value, hosts_enrichment){
                QString as_num = QString::number(value.toObject()["as_num"].toInt());
                QString as_org = value.toObject()["as_org"].toString();
                emit resultASN(as_num, as_org);
                log.resultsCount++;
            }
        }
        break;

    case AS:
        foreach(const QJsonValue &item, items){
            QString as_num = QString::number(item.toObject()["as_num"].toInt());
            QString as_org = item.toObject()["as_org"].toString();

            emit resultASN(as_num, as_org);
            log.resultsCount++;
        }
        break;

    case IPV4:
        foreach(const QJsonValue &item, items){
            QJsonObject isp_info = item.toObject()["isp_info"].toObject();

            QString as_num = QString::number(isp_info["as_num"].toInt());
            QString as_org = isp_info["as_org"].toString();

            emit resultASN(as_num, as_org);
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Spyse::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS:
        foreach(const QJsonValue &item, items){
            QJsonArray urls = item.toObject()["http_extract"].toObject()["links"].toArray();
            foreach(const QJsonValue &value, urls){
                emit resultURL(value.toObject()["url"].toString());
                log.resultsCount++;
            }
        }
    }

    this->end(reply);
}

void Spyse::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray items = document.object()["data"].toObject()["items"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case DOMAINS:
        foreach(const QJsonValue &item, items)
        {
            emit resultSubdomain(item["name"].toString());
            log.resultsCount++;

            /* from dns_records */
            QJsonObject dns_records = item.toObject()["dns_records"].toObject();
            QJsonArray ns = dns_records["NS"].toArray();
            foreach(const QJsonValue &value, ns){
                emit resultNS(value.toString());
                log.resultsCount++;
            }
            QJsonArray mx = dns_records["MX"].toArray();
            foreach(const QJsonValue &value, mx){
                emit resultMX(value.toString());
                log.resultsCount++;
            }
            QJsonArray cname = dns_records["CNAME"].toArray();
            foreach(const QJsonValue &value, cname){
                emit resultCNAME(value.toString());
                log.resultsCount++;
            }
            QJsonArray txt = dns_records["TXT"].toArray();
            foreach(const QJsonValue &value, txt){
                emit resultTXT(value.toString());
                log.resultsCount++;
            }
        }
    }

    this->end(reply);
}
