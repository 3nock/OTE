#include "IpRegistry.h"
#include "src/utils/Config.h"

#define BATCH_IP_LOOKUP 0
#define ORIGIN_IP_LOOKUP 1
#define SINGLE_IP_LOOKUP 2

/*
 * starts with 100,000 free lookups...
 */
IpRegistry::IpRegistry(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPREGISTRY;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpRegistry::replyFinishedRawJson);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_IPREGISTRY).toString();
}
IpRegistry::~IpRegistry(){
    delete manager;
}

void IpRegistry::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case SINGLE_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/"+target+"?key="+m_key);
            break;
        case BATCH_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/"+target+"?key="+m_key);
            break;
        case ORIGIN_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/?key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}
