#include "IpGeoLocation.h"
#include "src/utils/Config.h"

#define IP_GEOLOCATION 0
#define TIMEZONE 1

/*
 * 1,000 free queries...
 */
IpGeoLocation::IpGeoLocation(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "IpGeoLocation";

    if(args->outputRaw)
        connect(manager, &MyNetworkAccessManager::finished, this, &IpGeoLocation::replyFinishedRawJson);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipgeolocation").toString();
    Config::generalConfig().endGroup();
}
IpGeoLocation::~IpGeoLocation(){
    delete manager;
}

void IpGeoLocation::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case IP_GEOLOCATION:
            url.setUrl("https://api.ipgeolocation.io/ipgeo?apiKey="+m_key+"&ip="+args->target);
            break;
        case TIMEZONE:
            url.setUrl("https://api.ipgeolocation.io/timezone?apiKey="+m_key+"&ip="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
