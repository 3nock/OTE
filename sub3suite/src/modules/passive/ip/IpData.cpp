#include "IpData.h"
#include "src/utils/Config.h"

#define ASN_API 0
#define ASN_DATA 1
#define CURRENCY_DETECTION 2
#define IP_DATA 3
#define MOBILE_CARRIER_DETECTION 4
#define PROXY_TOR_THREAT_DETECTION 5
#define TIMEZONE_DETECTION 6

/*
 * 1,500/day for free-tier...
 */
IpData::IpData(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPDATA;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedRawJson);
    if(args.output_ASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedAsn);
    if(args.output_CIDR)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedCidr);
    if(args.output_EnumIP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedEnumIP);
    if(args.output_EnumASN)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedEnumASN);
    if(args.output_EnumASNPeers)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedEnumASNPeers);
    if(args.output_EnumASNPrefixes)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpData::replyFinishedEnumASNPrefixes);

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
            url.setUrl("https://api.ipdata.co/AS"+target+"/?api-key="+m_key);
            break;
        case ASN_DATA:
            url.setUrl("https://api.ipdata.co/"+target+"/asn?api-key="+m_key);
            break;
        case IP_DATA:
            url.setUrl("https://api.ipdata.co/"+target+"?api-key="+m_key);
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

    ///
    /// for enums
    ///

    if(args.output_EnumIP){
        url.setUrl("https://api.ipdata.co/"+target+"?api-key="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumASN || args.output_EnumASNPeers || args.output_EnumASNPrefixes){
        url.setUrl("https://api.ipdata.co/AS"+target+"/?api-key="+m_key);
        request.setUrl(url);
        manager->get(request);
        return;
    }

    ///
    /// for OSINT
    ///

    if(args.input_ASN){
        if(args.output_ASN || args.output_CIDR){
            url.setUrl("https://api.ipdata.co/AS"+target+"/?api-key="+m_key);
            request.setAttribute(QNetworkRequest::User, ASN_API);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_IP){
        if(args.output_ASN){
            // url.setUrl("https://api.ipdata.co/"+target+"/asn?api-key="+m_key);
            url.setUrl("https://api.ipdata.co/"+target+"?api-key="+m_key);
            request.setAttribute(QNetworkRequest::User, IP_DATA);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void IpData::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASN_API:
        foreach(const QJsonValue &value, mainObj["peers"].toArray())
            emit resultASN(value.toString(), "");
        break;

    case IP_DATA:
        QJsonObject asn = mainObj["asn"].toObject();
        emit resultASN(QString::number(asn["asn"].toInt()), asn["name"].toString());
        break;
    }

    end(reply);
}

void IpData::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    foreach(const QJsonValue &value, mainObj["ipv4_prefixes"].toArray())
        emit resultCIDR(value.toString());

    foreach(const QJsonValue &value, mainObj["ipv6_prefixes"].toArray())
        emit resultCIDR(value.toString());

    end(reply);
}

void IpData::replyFinishedEnumIP(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
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

void IpData::replyFinishedEnumASN(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    s3s_struct::ASN asn;
    asn.asn = target;

    asn.info_website = mainObj["domain"].toString();
    asn.info_country = mainObj["country"].toString();
    asn.info_name = mainObj["name"].toString();
    asn.info_asn = target;
    asn.rir_name = mainObj["registry"].toString();

    /* asn peers */
    foreach(const QJsonValue &value, mainObj["peers"].toArray())
        asn.peers.insert(value.toString());

    /* prefixes */
    foreach(const QJsonValue &value, mainObj["ipv4_prefixes"].toArray())
        asn.prefixes.insert(value.toString());

    foreach(const QJsonValue &value, mainObj["ipv6_prefixes"].toArray())
        asn.prefixes.insert(value.toString());

    emit resultEnumASN(asn);

    this->end(reply);
}

void IpData::replyFinishedEnumASNPeers(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    s3s_struct::ASN asn;
    asn.asn = target;

    foreach(const QJsonValue &value, mainObj["peers"].toArray())
        asn.peers.insert(value.toString());

    emit resultEnumASN(asn);

    this->end(reply);
}

void IpData::replyFinishedEnumASNPrefixes(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    s3s_struct::ASN asn;
    asn.asn = target;

    foreach(const QJsonValue &value, mainObj["ipv4_prefixes"].toArray())
        asn.prefixes.insert(value.toString());

    foreach(const QJsonValue &value, mainObj["ipv6_prefixes"].toArray())
        asn.prefixes.insert(value.toString());

    emit resultEnumASN(asn);

    this->end(reply);
}

