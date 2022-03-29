#include "Certspotter.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ISSUEANCES 0


Certspotter::Certspotter(ScanArgs args) : AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_CERTSPOTTER;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Certspotter::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Certspotter::replyFinishedSubdomain);
    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Certspotter::replyFinishedSSL);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_CERTSPOTTER).toString();
}
Certspotter::~Certspotter(){
    delete manager;
}

void Certspotter::start(){
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer "+m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case ISSUEANCES:
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+target+"&include_subdomains=true&expand=cert&expand=issuer&expand=dns_names");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+target+"&include_subdomains=true&expand=dns_names&match_wildcards=true");
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_SSL){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+target+"&include_subdomains=true&expand=cert");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void Certspotter::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array())
    {
        QJsonArray dns_names = value.toObject()["dns_names"].toArray();
        foreach(const QJsonValue &domain, dns_names){
            emit resultSubdomain(domain.toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void Certspotter::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array())
    {
        QJsonObject cert = value.toObject()["cert"].toObject();
        QString sha256 = cert["sha256"].toString();

        emit resultSSL(sha256);
        log.resultsCount++;
    }

    this->end(reply);
}
