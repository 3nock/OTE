#include "IpStack.h"
#include "src/utils/Config.h"

#define BULK_IP_LOOKUP 0
#define REQUESTER_IP_LOOKUP 1
#define STANDARD_IP_LOOKUP 2

/*
 * 100 per month for free-user, uses http instead of https for free users...
 */
IpStack::IpStack(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_IPSTACK;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpStack::replyFinishedRawJson);

    /* get api key */
    m_key = gConfig.keys.IpStack;
}
IpStack::~IpStack(){
    delete manager;
}

void IpStack::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case STANDARD_IP_LOOKUP:
            url.setUrl("http://api.ipstack.com/"+target+"?access_key="+m_key);
            break;
        case BULK_IP_LOOKUP:
            url.setUrl("http://api.ipstack.com/"+target+"?access_key="+m_key);
            break;
        case REQUESTER_IP_LOOKUP:
            url.setUrl("http://api.ipstack.com/check?access_key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}
