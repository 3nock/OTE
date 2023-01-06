#include "HaveIBeenPawned.h"
#include "src/utils/Config.h"

#define BREACHED_ACCOUNT 0
#define BREACHED_SITE 1
#define PASTES_ACCOUNT 2
#define SINGLE_BREACH 3

/*
 * for now only raw results
 */
HaveIBeenPawned::HaveIBeenPawned(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_HAVEIBEENPAWNED;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HaveIBeenPawned::replyFinishedRawJson);

    /* getting api key */
    m_key = gConfig.keys.HaveIBeenPawned;
}
HaveIBeenPawned::~HaveIBeenPawned(){
    delete manager;
}

void HaveIBeenPawned::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("hibp-api-key", m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case BREACHED_ACCOUNT:
            url.setUrl("https://haveibeenpwned.com/api/v3/breachedaccount/"+QUrl::toPercentEncoding(target));
            break;
        case BREACHED_SITE:
            url.setUrl("https://haveibeenpwned.com/api/v3/breaches?domain="+QUrl::toPercentEncoding(target));
            break;
        case PASTES_ACCOUNT:
            url.setUrl("https://haveibeenpwned.com/api/v3/pasteaccount/"+QUrl::toPercentEncoding(target));
            break;
        case SINGLE_BREACH:
            url.setUrl("https://haveibeenpwned.com/api/v3/breach/"+QUrl::toPercentEncoding(target));
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}
