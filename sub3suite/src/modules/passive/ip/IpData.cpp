#include "IpData.h"
#include "src/utils/Config.h"

#define ASN_API 0
#define ASN_DATA 1
#define CURRENCY_DETECTION 2
#define MOBILE_CARRIER_DETECTION 3
#define PROXY_TOR_THREAT_DETECTION 4
#define TIMEZONE_DETECTION 5

/*
 * 1,500/day for free-tier...
 */
IpData::IpData(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPDATA;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedRawJson);
    if(args.output_EnumIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedEnumIP);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_IPDATA).toString();
}
IpData::~IpData(){
    delete manager;
}

void IpData::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case ASN_API:
            url.setUrl("https://api.ipdata.co/"+target+"/asn?api-key="+m_key);
            break;
        case ASN_DATA:
            url.setUrl("https://api.ipdata.co/"+target+"/asn?api-key="+m_key);
            break;
        case CURRENCY_DETECTION:
            url.setUrl("https://api.ipdata.co/"+target+"/currency?api-key="+m_key);
            break;
        case MOBILE_CARRIER_DETECTION:
            url.setUrl("https://api.ipdata.co/"+target+"/carrier?api-key="+m_key);
            break;
        case PROXY_TOR_THREAT_DETECTION:
            url.setUrl("https://api.ipdata.co/"+target+"/threat?api-key="+m_key);
            break;
        case TIMEZONE_DETECTION:
            url.setUrl("https://api.ipdata.co/"+target+"/time_zone?api-key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumIP){
        url.setUrl("https://api.ipdata.co/"+target+"?api-key="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

void IpData::replyFinishedEnumIP(QNetworkReply *reply){
    if(reply->error()){
        quitThread();
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    s3s_struct::IP ip;
    ip.ip = target;

    ip.info_ip = mainObj["ip"].toString();
    ip.info_region = mainObj["region"].toString();
    ip.info_city = mainObj["city"].toString();
    ip.info_countryCode = mainObj["country_code"].toString();
    ip.info_countryName = mainObj["country_name"].toString();

    QString latitude = QString::number(mainObj["latitude"].toDouble());
    QString longitude = QString::number(mainObj["longitude"].toDouble());
    ip.info_geoLocation = latitude+","+longitude;

    QJsonObject time_zone = mainObj["time_zone"].toObject();
    ip.info_timezone = time_zone["name"].toString();

    QJsonObject asn = mainObj["asn"].toObject();
    ip.asnInfo_asn = QString::number(asn["asn"].toInt());
    ip.asnInfo_name = asn["name"].toString();
    ip.asnInfo_route = asn["route"].toString();
    ip.info_organization = asn["name"].toString();

    QJsonObject privacy = mainObj["security"].toObject();
    ip.privacyInfo_tor = privacy["is_tor"].toBool();
    ip.privacyInfo_proxy = privacy["is_proxy"].toBool();
    ip.privacyInfo_anonymous = privacy["is_anonymous"].toBool();
    ip.privacyInfo_attacker = privacy["is_known_attacker"].toBool();
    ip.privacyInfo_abuser = privacy["is_known_abuser"].toBool();
    ip.privacyInfo_threat = privacy["is_threat"].toBool();
    emit resultEnumIP(ip);

    end(reply);
}

