#include "Host.h"
#include "src/utils/Config.h"

#define DNS 0
#define FULL 1
#define RELATED 2
#define WEB 3

Host::Host(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Host";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Host::replyFinishedRaw);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("host").toString();
    Config::generalConfig().endGroup();
}
Host::~Host(){
    delete manager;
}

void Host::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case DNS:
            url.setUrl("https://host.io/api/dns/"+args->target+"?token="+m_key);
            break;
        case FULL:
            url.setUrl("https://host.io/api/full/"+args->target+"?token="+m_key);
            break;
        case RELATED:
            url.setUrl("https://host.io/api/related/"+args->target+"?token="+m_key);
            break;
        case WEB:
            url.setUrl("https://host.io/api/web/"+args->target+"?token="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
