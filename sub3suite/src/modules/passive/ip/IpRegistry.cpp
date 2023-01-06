#include "IpRegistry.h"
#include "src/utils/Config.h"

#define BATCH_IP_LOOKUP 0
#define ORIGIN_IP_LOOKUP 1
#define SINGLE_IP_LOOKUP 2

/*
 * starts with 100,000 free lookups...
 */
IpRegistry::IpRegistry(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPREGISTRY;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpRegistry::replyFinishedRawJson);
    if(args.output_EnumIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpRegistry::replyFinishedEnumIP);

    /* get api key */
    m_key = gConfig.keys.IpRegistry;
}
IpRegistry::~IpRegistry(){
    delete manager;
}

void IpRegistry::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case SINGLE_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/"+target+"?key="+m_key);
            break;
        case BATCH_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/"+target+"?key="+m_key);
            break;
        case ORIGIN_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/?key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumIP) {
        url.setUrl("https://api.ipregistry.co/"+target+"?key="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

void IpRegistry::replyFinishedEnumIP(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    s3s_struct::IP ip;
    ip.ip = target;

    ip.info_ip = mainObj["ip"].toString();

    QJsonObject region = mainObj["region"].toObject();
    ip.info_region = region["name"].toString();

    QJsonObject location = mainObj["location"].toObject();
    ip.info_city = location["city"].toString();
    QString latitude = QString::number(location["latitude"].toDouble());
    QString longitude = QString::number(location["longitude"].toDouble());
    ip.info_geoLocation = latitude+","+longitude;

    QJsonObject country = location["country"].toObject();
    ip.info_countryCode = country["code"].toString();
    ip.info_countryName = country["name"].toString();

    QJsonObject time_zone = mainObj["time_zone"].toObject();
    ip.info_timezone = time_zone["name"].toString();

    QJsonObject connection = mainObj["connection"].toObject();
    ip.asnInfo_asn = QString::number(connection["asn"].toInt());
    ip.asnInfo_name = connection["organization"].toString();
    ip.asnInfo_route = connection["route"].toString();
    ip.info_organization = connection["organization"].toString();

    QJsonObject company = mainObj["company"].toObject();
    ip.companyInfo_name = company["name"].toString();
    ip.companyInfo_domain = company["domain"].toString();

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
