#include "Ipfy.h"
#include "src/utils/Config.h"

#define IP_GEOLOCATION_DOMAIN 0
#define IP_GEOLOCATION_EMAIL 1
#define IP_GEOLOCATION_IP 2
#define PUBLIC_IPV4 3
#define PUBLIC_IPV6 4

Ipfy::Ipfy(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPFY;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Ipfy::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Ipfy::replyFinishedSubdomain);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Ipfy::replyFinishedIp);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Ipfy::replyFinishedAsn);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Ipfy::replyFinishedCidr);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_IPFY).toString();
}
Ipfy::~Ipfy(){
    delete manager;
}

void Ipfy::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case PUBLIC_IPV4:
            url.setUrl("https://api.ipify.org?format=json");
            break;
        case PUBLIC_IPV6:
            url.setUrl("https://api64.ipify.org?format=json");
            break;
        case IP_GEOLOCATION_DOMAIN:
            url.setUrl("https://geo.ipify.org/api/v2/country?apiKey="+m_key+"&domain="+target);
            break;
        case IP_GEOLOCATION_EMAIL:
            url.setUrl("https://geo.ipify.org/api/v2/country?apiKey="+m_key+"&email="+target);
            break;
        case IP_GEOLOCATION_IP:
            url.setUrl("https://geo.ipify.org/api/v2/country?apiKey="+m_key+"&ipAddress="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_IP){
        if(args.output_Hostname || args.output_ASN || args.output_CIDR){
            url.setUrl("https://geo.ipify.org/api/v2/country?apiKey="+m_key+"&ipAddress="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Domain){
        if(args.output_IP || args.output_Hostname || args.output_ASN || args.output_CIDR){
            url.setUrl("https://geo.ipify.org/api/v2/country?apiKey="+m_key+"&domain="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void Ipfy::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    foreach(const QJsonValue &domain, mainObj["domains"].toArray()){
        emit resultSubdomain(domain.toString());
        log.resultsCount++;
    }

    this->end(reply);
}

void Ipfy::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject as = document.object()["as"].toObject();

    emit resultASN(QString::number(as["asn"].toInt()), as["name"].toString());
    log.resultsCount++;

    this->end(reply);
}

void Ipfy::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    emit resultIP(mainObj["ip"].toString());
    log.resultsCount++;

    this->end(reply);
}

void Ipfy::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject as = document.object()["as"].toObject();

    emit resultCIDR(as["route"].toString());
    log.resultsCount++;

    this->end(reply);
}
