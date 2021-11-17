#include "HaveIBeenPawned.h"
#include "src/utils/Config.h"

#define BREACHED_ACCOUNT 0
#define BREACHED_SITE 1
#define PASTES_ACCOUNT 2
#define SINGLE_BREACH 3

/*
 * url encode the email...
 */
HaveIBeenPawned::HaveIBeenPawned(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "HaveIBeenPawned";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &HaveIBeenPawned::replyFinishedRaw);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("haveibeenpawned").toString();
    Config::generalConfig().endGroup();
}
HaveIBeenPawned::~HaveIBeenPawned(){
    delete manager;
}

void HaveIBeenPawned::start(){
    QNetworkRequest request;
    request.setRawHeader("hibp-api-key", m_key.toUtf8());

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case BREACHED_ACCOUNT:
            url.setUrl("https://haveibeenpwned.com/api/v3/breachedaccount/"+args->target);
            break;
        case BREACHED_SITE:
            url.setUrl("GET https://haveibeenpwned.com/api/v3/breaches?domain="+args->target);
            break;
        case PASTES_ACCOUNT:
            url.setUrl("https://haveibeenpwned.com/api/v3/pasteaccount/"+args->target);
            break;
        case SINGLE_BREACH:
            url.setUrl("https://haveibeenpwned.com/api/v3/breach/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
