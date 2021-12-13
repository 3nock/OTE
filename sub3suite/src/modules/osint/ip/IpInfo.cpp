#include "IpInfo.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASN 0
#define HOSTED_DOMAINS 1
#define IP 2
#define RANGES 3

/*
 * 50,000 requests per month
 */
IpInfo::IpInfo(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "IpInfo";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &IpInfo::replyFinishedRawJson);
    if(args.outputInfo)
        connect(manager, &NetworkAccessManager::finished, this, &IpInfo::replyFinishedInfo);
    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &IpInfo::replyFinishedSubdomain);
    ///
    /// getting the api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipinfo").toString();
    Config::generalConfig().endGroup();
}
IpInfo::~IpInfo(){
    delete manager;
}

void IpInfo::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case IP:
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
        activeRequests++;
        return;
    }

    if(args.outputInfo){
        url.setUrl("https://ipinfo.io/"+target+"/json?token="+m_key);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputIp){
        if(args.outputSubdomain){
            url.setUrl("https://ipinfo.io/domains"+target+"/json?token="+m_key);
            request.setAttribute(QNetworkRequest::User, HOSTED_DOMAINS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void IpInfo::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray domains = document.object()["domains"].toArray();

    if(QUERY_TYPE == HOSTED_DOMAINS){
        foreach(const QJsonValue &value, domains){
            emit subdomain(value.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void IpInfo::replyFinishedInfo(QNetworkReply *reply){
    if(reply->error()){
        quitThread();
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    /*

    args.ipModel->info_ip->setText(mainObj["ip"].toString());
    args.ipModel->info_city->setText(mainObj["city"].toString());
    args.ipModel->info_region->setText(mainObj["region"].toString());
    args.ipModel->info_countryCode->setText(mainObj["country"].toString());
    args.ipModel->info_geoLocation->setText(mainObj["loc"].toString());
    args.ipModel->info_timezone->setText(mainObj["timezone"].toString());
    args.ipModel->info_organization->setText(mainObj["org"].toString());

    ///
    /// if basic plan or higher...
    ///
    if(!mainObj["asn"].isNull() || !mainObj["asn"].isUndefined()){
        QJsonObject asn = mainObj["asn"].toObject();

        args.ipModel->asnInfo_asn->setText(asn["asn"].toString());
        args.ipModel->asnInfo_name->setText(asn["name"].toString());
        args.ipModel->asnInfo_domain->setText(asn["domain"].toString());
        args.ipModel->asnInfo_route->setText(asn["route"].toString());
        args.ipModel->asnInfo_type->setText(asn["type"].toString());
    }

    ///
    /// if bussiness plan or higher...
    ///
    if(!mainObj["company"].isNull() || !mainObj["company"].isUndefined()){
        QJsonObject company = mainObj["company"].toObject();

        args.ipModel->companyInfo_name->setText(company["name"].toString());
        args.ipModel->companyInfo_domain->setText(company["domain"].toString());
        args.ipModel->companyInfo_type->setText(company["type"].toString());
    }

    if(!mainObj["privacy"].isNull() || !mainObj["privacy"].isUndefined()){
        QJsonObject privacy = mainObj["privacy"].toObject();

        args.ipModel->privacyInfo_tor->setText(privacy["tor"].toString());
        args.ipModel->privacyInfo_vpn->setText(privacy["vpn"].toString());
        args.ipModel->privacyInfo_proxy->setText(privacy["proxy"].toString());
        args.ipModel->privacyInfo_relay->setText(privacy["relay"].toString());
        args.ipModel->privacyInfo_hosting->setText(privacy["hosting"].toString());
    }

    if(!mainObj["abuse"].isNull() || !mainObj["abuse"].isUndefined()){
        QJsonObject abuse = mainObj["abuse"].toObject();

        args.ipModel->abuseInfo_name->setText(abuse["name"].toString());
        args.ipModel->abuseInfo_email->setText(abuse["email"].toString());
        args.ipModel->abuseInfo_phone->setText(abuse["phone"].toString());
        args.ipModel->abuseInfo_address->setText(abuse["address"].toString());
        args.ipModel->abuseInfo_country->setText(abuse["country"].toString());
        args.ipModel->abuseInfo_network->setText(abuse["network"].toString());
    }

    if(!mainObj["domains"].isNull() || !mainObj["domains"].isUndefined()){
        QJsonArray domains = mainObj["domains"].toObject()["domains"].toArray();
        foreach(const QJsonValue &value, domains){
            args.ipModel->domains->appendRow(new QStandardItem(value.toString()));
        }
    }
    */

    emit quitThread();
}
