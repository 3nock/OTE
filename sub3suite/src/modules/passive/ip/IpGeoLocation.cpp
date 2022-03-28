#include "IpGeoLocation.h"
#include "src/utils/Config.h"

#define IP_GEOLOCATION 0
#define TIMEZONE 1

/*
 * 1,000 free queries...
 */
IpGeoLocation::IpGeoLocation(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_IPGEOLOCATION;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpGeoLocation::replyFinishedRawJson);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_IPGEOLOCATION).toString();
}
IpGeoLocation::~IpGeoLocation(){
    delete manager;
}

void IpGeoLocation::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case IP_GEOLOCATION:
            url.setUrl("https://api.ipgeolocation.io/ipgeo?apiKey="+m_key+"&ip="+target);
            break;
        case TIMEZONE:
            url.setUrl("https://api.ipgeolocation.io/timezone?apiKey="+m_key+"&ip="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
