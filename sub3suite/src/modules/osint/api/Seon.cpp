#include "Seon.h"
#include "src/utils/Config.h"

#define EMAIL 0
#define EMAIL_VERIFICATION 1
#define PHONE 2
#define IP 3

Seon::Seon(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Seon";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Seon::replyFinishedRawJson);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("seon").toString();
    Config::generalConfig().endGroup();
}
Seon::~Seon(){
    delete manager;
}

void Seon::start(){
    QNetworkRequest request;
    request.setRawHeader("X-API-KEY", m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case EMAIL:
            url.setUrl("https://api.seon.io/SeonRestService/email-api/v2.1/"+target);
            break;
        case EMAIL_VERIFICATION:
            url.setUrl("https://api.seon.io/SeonRestService/email-verification/v1.0/"+target);
            break;
        case PHONE:
            url.setUrl("https://api.seon.io/SeonRestService/phone-api/v1.2/"+target);
            break;
        case IP:
            url.setUrl("https://api.seon.io/SeonRestService/ip-api/v1.1/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
