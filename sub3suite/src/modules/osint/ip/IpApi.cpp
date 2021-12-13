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
    manager = new NetworkAccessManager(this);
    log.moduleName = "IpApi";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &IpApi::replyFinishedRawJson);
    if(args.outputInfo)
        connect(manager, &NetworkAccessManager::finished, this, &IpApi::replyFinishedInfo);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipapi").toString();
    Config::generalConfig().endGroup();
}
IpApi::~IpApi(){
    delete manager;
}

void IpApi::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
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
        activeRequests++;
        return;
    }

    if(args.outputInfo){
        url.setUrl("http://api.ipapi.com/api/"+target+"?access_key="+m_key);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void IpApi::replyFinishedInfo(QNetworkReply *reply){
    if(reply->error()){
        quitThread();
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    /*
    args.ipModel->info_ip->setText(mainObj["ip"].toString());
    args.ipModel->info_type->setText(mainObj["type"].toString());
    args.ipModel->info_host->setText(mainObj["hostname"].toString());
    args.ipModel->info_city->setText(mainObj["city"].toString());
    args.ipModel->info_region->setText(mainObj["region_name"].toString());
    args.ipModel->info_countryCode->setText(mainObj["country_code"].toString());
    args.ipModel->info_countryName->setText(mainObj["country_name"].toString());
    args.ipModel->info_zip->setText(mainObj["zip"].toString());
    ///
    /// for location...
    ///
    QString latitude = QString::number(mainObj["latitude"].toDouble());
    QString longitude = QString::number(mainObj["longitude"].toDouble());
    args.ipModel->info_geoLocation->setText(latitude+","+longitude);

    ///
    /// for timezone...
    ///
    if(!mainObj["time_zone"].isNull() || !mainObj["time_zone"].isUndefined()){
        QString timezone = mainObj["time_zone"].toObject()["id"].toString();
        args.ipModel->info_timezone->setText(timezone);
    }
    ///
    /// for currency...
    ///
    if(!mainObj["currency"].isNull() || !mainObj["currency"].isUndefined()){
        QString currency = mainObj["currency"].toObject()["name"].toString();
        args.ipModel->info_currency->setText(currency);
    }

    ///
    /// for connection...
    ///
    if(!mainObj["connection"].isNull() || !mainObj["connection"].isUndefined()){
        QString asn = QString::number(mainObj["connection"].toObject()["asn"].toInt());
        args.ipModel->asnInfo_asn->setText(asn);
    }

    ///
    /// for privacy...
    ///
    if(!mainObj["security"].isNull() || !mainObj["security"].isUndefined()){
        QJsonObject security = mainObj["security"].toObject();
        args.ipModel->privacyInfo_proxy->setText(security["is_proxy"].toString());
        args.ipModel->privacyInfo_tor->setText(security["is_tor"].toString());
        args.ipModel->privacyInfo_threatLevel->setText(security["threat_level"].toString());
    }
    */

    emit quitThread();
}
