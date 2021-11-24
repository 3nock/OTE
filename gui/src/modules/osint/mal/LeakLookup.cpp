#include "LeakLookup.h"
#include "src/utils/Config.h"

#define DOMAINS 0
#define EMAIL 1
#define IP 2
#define USERNAME 3

/*
 * something is wrong!...
 */
LeakLookup::LeakLookup(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "LeakLookup";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &LeakLookup::replyFinishedRaw);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("leaklookup").toString();
    Config::generalConfig().endGroup();
}
LeakLookup::~LeakLookup(){
    delete manager;
}

void LeakLookup::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case DOMAINS:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=domain&query="+args->target);
            break;
        case EMAIL:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=email_address&query="+args->target);
            break;
        case IP:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=ipaddress&query="+args->target);
            break;
        case USERNAME:
            url.setUrl("https://leak-lookup.com/api/search/?key="+m_key+"&type=username&query="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
