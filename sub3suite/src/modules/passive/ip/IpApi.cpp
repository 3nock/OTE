#include "IpApi.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define BULK_LOOKUP 0
#define ORIGIN_LOOKUP 1
#define STANDARD_LOOKUP 2

/*
 * 1,000 per month for free-user...
 */
IpApi::IpApi(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPAPI;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpApi::replyFinishedRawJson);
    if(args.output_EnumIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpApi::replyFinishedEnumIP);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_IPAPI).toString();
}
IpApi::~IpApi(){
    delete manager;
}

void IpApi::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case STANDARD_LOOKUP:
            url.setUrl("http://api.ipapi.com/api/"+target+"?access_key="+m_key);
            break;
        case BULK_LOOKUP:
            url.setUrl("http://api.ipapi.com/api/"+target+"?access_key="+m_key);
            break;
        case ORIGIN_LOOKUP:
            url.setUrl("http://api.ipapi.com/api/check?access_key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumIP){
        url.setUrl("http://api.ipapi.com/api/"+target+"?access_key="+m_key);
        request.setUrl(url);
        manager->get(request);
    }
}

void IpApi::replyFinishedEnumIP(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    s3s_struct::IP ip;
    ip.ip = target;

    ip.info_ip = mainObj["ip"].toString();
    ip.info_type = mainObj["type"].toString();
    ip.info_city = mainObj["city"].toString();
    ip.info_region = mainObj["region_name"].toString();
    ip.info_countryCode = mainObj["country_code"].toString();
    ip.info_countryName = mainObj["country_name"].toString();
    ip.info_zip = mainObj["zip"].toString();

    /* for location... */
    QString latitude = QString::number(mainObj["latitude"].toDouble());
    QString longitude = QString::number(mainObj["longitude"].toDouble());
    ip.info_geoLocation = latitude+","+longitude;

    /* for timezone... */
    QString timezone = mainObj["time_zone"].toObject()["id"].toString();
    ip.info_timezone = timezone;

    /* for connection... */
    QString asn = QString::number(mainObj["connection"].toObject()["asn"].toInt());
    ip.asnInfo_asn = asn;

    /* for privacy... */
    QJsonObject security = mainObj["security"].toObject();
    ip.privacyInfo_proxy = security["is_proxy"].toBool();
    ip.privacyInfo_tor = security["is_tor"].toBool();

    emit resultEnumIP(ip);

    this->end(reply);
}
