#include "IpInfo.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASN 0
#define HOSTED_DOMAINS 1
#define IPINFO 2
#define RANGES 3

/*
 * 50,000 requests per month...
 * has different plans and different response for each plan...
 */
IpInfo::IpInfo(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPINFO;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpInfo::replyFinishedRawJson);
    if(args.output_EnumIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpInfo::replyFinishedEnumIP);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpInfo::replyFinishedSubdomain);

    /* getting the api key */
    m_key = APIKEY.value(OSINT_MODULE_IPINFO).toString();
}
IpInfo::~IpInfo(){
    delete manager;
}

void IpInfo::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case IPINFO:
            url.setUrl("https://ipinfo.io/"+target+"/json?token="+m_key);
            break;
        case ASN:
            url.setUrl("https://ipinfo.io/AS"+target+"/json?token="+m_key);
            break;
        case RANGES:
            url.setUrl("https://ipinfo.io/ranges/"+target+"/json?token="+m_key);
            break;
        case HOSTED_DOMAINS:
            url.setUrl("https://ipinfo.io/domains"+target+"/json?token="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumIP){
        url.setUrl("https://ipinfo.io/"+target+"/json?token="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_IP){
        if(args.output_Hostname){
            url.setUrl("https://ipinfo.io/domains"+target+"/json?token="+m_key);
            request.setAttribute(QNetworkRequest::User, HOSTED_DOMAINS);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void IpInfo::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray domains = document.object()["domains"].toArray();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOSTED_DOMAINS:
        foreach(const QJsonValue &value, domains){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
    }

    this->end(reply);
}

void IpInfo::replyFinishedEnumIP(QNetworkReply *reply){
    if(reply->error()){
        quitThread();
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    s3s_struct::IP ip;
    ip.ip = target;

    ip.info_ip = mainObj["ip"].toString();
    ip.info_city = mainObj["city"].toString();
    ip.info_region = mainObj["region"].toString();
    ip.info_countryCode = mainObj["country"].toString();
    ip.info_geoLocation = mainObj["loc"].toString();
    ip.info_timezone = mainObj["timezone"].toString();
    ip.info_organization = mainObj["org"].toString();

    /* if basic plan or higher... */
    QJsonObject asn = mainObj["asn"].toObject();
    ip.asnInfo_asn = asn["asn"].toString();
    ip.asnInfo_name = asn["name"].toString();
    ip.asnInfo_route = asn["route"].toString();

    /* if bussiness plan or higher... */
    QJsonObject company = mainObj["company"].toObject();
    ip.companyInfo_name = company["name"].toString();
    ip.companyInfo_domain = company["domain"].toString();

    QJsonObject privacy = mainObj["privacy"].toObject();
    ip.privacyInfo_vpn = privacy["vpn"].toBool();
    ip.privacyInfo_proxy = privacy["proxy"].toBool();
    ip.privacyInfo_tor = privacy["tor"].toBool();
    emit resultEnumIP(ip);

    end(reply);
}
