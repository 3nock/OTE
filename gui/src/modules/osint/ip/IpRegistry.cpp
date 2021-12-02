#include "IpRegistry.h"
#include "src/utils/Config.h"

#define BATCH_IP_LOOKUP 0
#define ORIGIN_IP_LOOKUP 1
#define SINGLE_IP_LOOKUP 2

/*
 * starts with 100,000 free lookups...
 */
IpRegistry::IpRegistry(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "IpRegistry";

    if(args->outputRaw)
        connect(manager, &MyNetworkAccessManager::finished, this, &IpRegistry::replyFinishedRawJson);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipregistry").toString();
    Config::generalConfig().endGroup();
}
IpRegistry::~IpRegistry(){
    delete manager;
}

void IpRegistry::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case SINGLE_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/"+args->target+"?key="+m_key);
            break;
        case BATCH_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/"+args->target+"?key="+m_key);
            break;
        case ORIGIN_IP_LOOKUP:
            url.setUrl("https://api.ipregistry.co/?key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
