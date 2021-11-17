#include "JsonWhois.h"
#include "src/utils/Config.h"

#define SCREENSHOT 0
#define SOCIAL_DATA 1
#define WHOIS 2


JsonWhois::JsonWhois(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "JsonWhois";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &JsonWhois::replyFinishedRaw);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("JsonWhois").toString();
    Config::generalConfig().endGroup();
}
JsonWhois::~JsonWhois(){
    delete manager;
}

void JsonWhois::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", "Token token="+m_key.toUtf8());

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case SCREENSHOT:
            url.setUrl("https://jsonwhois/api/v1/screenshot?domain="+args->target);
            break;
        case SOCIAL_DATA:
            url.setUrl("https://jsonwhois/api/v1/social?domain="+args->target);
            break;
        case WHOIS:
            url.setUrl("https://jsonwhois/api/v1/whois?domain="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

