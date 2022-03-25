#include "Seon.h"
#include "src/utils/Config.h"

#define EMAIL 0
#define EMAIL_VERIFICATION 1
#define PHONE 2
#define IP 3

Seon::Seon(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_SEON;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Seon::replyFinishedRawJson);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_SEON).toString();
}
Seon::~Seon(){
    delete manager;
}

void Seon::start(){
    QNetworkRequest request;
    request.setRawHeader("X-API-KEY", m_key.toUtf8());

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
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
