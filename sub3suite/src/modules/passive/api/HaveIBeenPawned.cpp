#include "HaveIBeenPawned.h"
#include "src/utils/Config.h"

#define BREACHED_ACCOUNT 0
#define BREACHED_SITE 1
#define PASTES_ACCOUNT 2
#define SINGLE_BREACH 3

/*
 * url encode the email...
 */
HaveIBeenPawned::HaveIBeenPawned(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "HaveIBeenPawned";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HaveIBeenPawned::replyFinishedRawJson);
    ///
    /// getting api key...
    ///
    
    m_key = APIKEY.value("haveibeenpawned").toString();
    
}
HaveIBeenPawned::~HaveIBeenPawned(){
    delete manager;
}

void HaveIBeenPawned::start(){
    QNetworkRequest request;
    request.setRawHeader("hibp-api-key", m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case BREACHED_ACCOUNT:
            url.setUrl("https://haveibeenpwned.com/api/v3/breachedaccount/"+target);
            break;
        case BREACHED_SITE:
            url.setUrl("GET https://haveibeenpwned.com/api/v3/breaches?domain="+target);
            break;
        case PASTES_ACCOUNT:
            url.setUrl("https://haveibeenpwned.com/api/v3/pasteaccount/"+target);
            break;
        case SINGLE_BREACH:
            url.setUrl("https://haveibeenpwned.com/api/v3/breach/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
