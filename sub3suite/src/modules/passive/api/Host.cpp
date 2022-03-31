#include "Host.h"
#include "src/utils/Config.h"

#define DNS 0
#define FULL 1
#define RELATED 2
#define WEB 3

/*
 * for now only raw results
 */
Host::Host(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_HOST;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Host::replyFinishedRawJson);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_HOST).toString();
}
Host::~Host(){
    delete manager;
}

void Host::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case DNS:
            url.setUrl("https://host.io/api/dns/"+target+"?token="+m_key);
            break;
        case FULL:
            url.setUrl("https://host.io/api/full/"+target+"?token="+m_key);
            break;
        case RELATED:
            url.setUrl("https://host.io/api/related/"+target+"?token="+m_key);
            break;
        case WEB:
            url.setUrl("https://host.io/api/web/"+target+"?token="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}
