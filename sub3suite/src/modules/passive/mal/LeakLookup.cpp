#include "LeakLookup.h"
#include "src/utils/Config.h"

#define DOMAINS 0
#define EMAIL 1
#define IP 2
#define USERNAME 3

/*
 * something is wrong!...
 */
LeakLookup::LeakLookup(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_LEAKLOOKUP;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &LeakLookup::replyFinishedRawJson);

    /* get api key */
    m_key = gConfig.keys.LeakLookup;
}
LeakLookup::~LeakLookup(){
    delete manager;
}

void LeakLookup::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case DOMAINS:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=domain&query="+target);
            break;
        case EMAIL:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=email_address&query="+target);
            break;
        case IP:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=ipaddress&query="+target);
            break;
        case USERNAME:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=username&query="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}
