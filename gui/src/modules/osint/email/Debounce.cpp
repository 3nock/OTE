#include "Debounce.h"
#include "src/utils/Config.h"

#define ACCOUNT_BALANCE 0
#define API_USAGE_HISTORY 1
#define DISPOSABLE_EMAIL 2
#define SINGLE_VALIDATION 3


Debounce::Debounce(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Debounce";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Debounce::replyFinishedRawJson);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("debounce").toString();
    Config::generalConfig().endGroup();
}
Debounce::~Debounce(){
    delete manager;
}

void Debounce::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch(args->rawOption){
        case ACCOUNT_BALANCE:
            url.setUrl("https://api.debounce.io/v1/balance?api="+m_key);
            break;
        case API_USAGE_HISTORY:
            url.setUrl("https://api.debounce.io/v1/usage?api="+m_key);
            break;
        case DISPOSABLE_EMAIL:
            url.setUrl("https://disposable.debounce.io?email="+args->target);
            break;
        case SINGLE_VALIDATION:
            url.setUrl("https://api.debounce.io/v1?api="+m_key+"&email="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
