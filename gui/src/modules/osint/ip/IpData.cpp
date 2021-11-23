#include "IpData.h"
#include "src/utils/Config.h"

#define ASN_DATA 0
#define CURRENCY_DETECTION 1
#define MOBILE_CARRIER_DETECTION 2
#define PROXY_TOR_THREAT_DETECTION 3
#define TIMEZONE_DETECTION 4

/*
 * 1,500/day for free-tier...
 */
IpData::IpData(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "IpData";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &IpData::replyFinishedRaw);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipdata").toString();
    Config::generalConfig().endGroup();
}
IpData::~IpData(){
    delete manager;
}

void IpData::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case ASN_DATA:
            url.setUrl("https://api.ipdata.co/"+args->target+"/asn?api-key="+m_key);
            break;
        case CURRENCY_DETECTION:
            url.setUrl("https://api.ipdata.co/"+args->target+"/currency?api-key="+m_key);
            break;
        case MOBILE_CARRIER_DETECTION:
            url.setUrl("https://api.ipdata.co/"+args->target+"/carrier?api-key="+m_key);
            break;
        case PROXY_TOR_THREAT_DETECTION:
            url.setUrl("https://api.ipdata.co/"+args->target+"/threat?api-key="+m_key);
            break;
        case TIMEZONE_DETECTION:
            url.setUrl("https://api.ipdata.co/"+args->target+"/time_zone?api-key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
