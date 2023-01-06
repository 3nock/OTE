#include "Wappalyzer.h"
#include "src/utils/Config.h"

#define BALANCE 0
#define EMAIL_VERIFY 1
#define TECHNOLOGY_LOOKUP 2


/*
 * can query multiple urls at a time
 * for now only raw results with default options
 */
Wappalyzer::Wappalyzer(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_WAPPALYZER;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Wappalyzer::replyFinishedRawJson);

    /* getting api key */
    m_key = gConfig.keys.Wappalyzer;
}
Wappalyzer::~Wappalyzer(){
    delete manager;
}

void Wappalyzer::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("x-api-key", m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case BALANCE:
            url.setUrl("https://api.wappalyzer.com/v2/credits/balance/");
            break;
        case EMAIL_VERIFY:
            url.setUrl("https://api.wappalyzer.com/v2/verify/?email="+target);
            break;
        case TECHNOLOGY_LOOKUP:
            url.setUrl("https://api.wappalyzer.com/v2/lookup/?urls="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

