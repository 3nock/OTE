#include "IpQualityScore.h"
#include "src/utils/Config.h"

#define EMAIL_VALIDATION 0
#define MALICIOUS_URL_SCANNER 1
#define PHONE_VALIDATION 2
#define PROXY_VPN_DETECTION 3

/*
 * 5,000 per month for free-user...
 * url encode the url...
 */
IpQualityScore::IpQualityScore(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "IpQualityScore";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &IpQualityScore::replyFinishedRawJson);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipqualityscore").toString();
    Config::generalConfig().endGroup();
}
IpQualityScore::~IpQualityScore(){
    delete manager;
}

void IpQualityScore::start(){
    QNetworkRequest request;
    request.setRawHeader("Key", m_key.toUtf8());
    request.setRawHeader("Accept", "application/json");

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case EMAIL_VALIDATION:
            url.setUrl("https://ipqualityscore.com/api/json/email/"+m_key+"/"+args->target);
            break;
        case MALICIOUS_URL_SCANNER:
            url.setUrl("https://ipqualityscore.com/api/json/url/"+m_key+"/"+args->target);
            break;
        case PHONE_VALIDATION:
            url.setUrl("https://ipqualityscore.com/api/json/phone/"+m_key+"/"+args->target);
            break;
        case PROXY_VPN_DETECTION:
            url.setUrl("https://ipqualityscore.com/api/json/ip/"+m_key+"/"+args->target+"?strictness=0&allow_public_access_points=true&fast=true&lighter_penalties=true&mobile=true");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

/*
    reporting api, also requestsList

The URL below is an example of how to report an IP address.
https://ipqualityscore.com/api/json/report/YOUR_API_KEY_HERE?ip=197.187.203.251
The URL below is an example of how to report an email address.
https://ipqualityscore.com/api/json/report/YOUR_API_KEY_HERE?email=noreply@ipqualityscore.com
The URL below is an example of how to report a previous request.
https://ipqualityscore.com/api/json/report/YOUR_API_KEY_HERE?request_id=sn342Xd22d
The URL below is an example of how to report a phone number.
https://ipqualityscore.com/api/json/report/YOUR_API_KEY_HERE?country=US&phone=8007132618

*/
