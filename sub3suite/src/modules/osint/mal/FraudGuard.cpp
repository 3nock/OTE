#include "FraudGuard.h"
#include "src/utils/Config.h"

#define GET_CUSTOM_BLACKLIST 0
#define GET_CUSTOM_GEOBLOCK 1
#define GET_CUSTOM_WHITELIST 2
#define SPECIFIC_HOST_REP_V2 3
#define SPECIFIC_IP_GEO_ISP_ORG 4
#define SPECIFIC_IP_REP_V1 5
#define SPECIFIC_IP_REP_V2 6

/*
 * 1,000 per month for free-user...
 * has lots of apis i havent implemented...
 */
FraudGuard::FraudGuard(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "FraudGuard";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &FraudGuard::replyFinishedRawJson);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_user = Config::generalConfig().value("fraudguard_user").toString();
    m_pass = Config::generalConfig().value("fraudguard_pass").toString();
    Config::generalConfig().endGroup();
}
FraudGuard::~FraudGuard(){
    delete manager;
}

void FraudGuard::start(){
    QNetworkRequest request;
    /* Basic Authentication */
    QString credentialsString = QString("%1:%2").arg(m_user).arg(m_pass);
    QByteArray credentialsArray = credentialsString.toLocal8Bit().toBase64();
    QString headerData = "Basic " + credentialsArray;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case GET_CUSTOM_GEOBLOCK:
            url.setUrl("https://@api.fraudguard.io/geoblock");
            break;
        case GET_CUSTOM_BLACKLIST:
            url.setUrl("https://@api.fraudguard.io/whitelist/0");
            break;
        case GET_CUSTOM_WHITELIST:
            url.setUrl("https://@api.fraudguard.io/blacklist/0");
            break;
        case SPECIFIC_IP_REP_V1:
            url.setUrl("https://@api.fraudguard.io/ip/"+target);
            break;
        case SPECIFIC_IP_REP_V2:
            url.setUrl("https://@api.fraudguard.io/v2/ip/"+target);
            break;
        case SPECIFIC_HOST_REP_V2:
            url.setUrl("https://@api.fraudguard.io/v2/hostname/"+target);
            break;
        case SPECIFIC_IP_GEO_ISP_ORG:
            url.setUrl("https://@api.fraudguard.io/v2/ip/geolookup/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
