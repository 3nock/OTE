#include "JsonWhois.h"
#include "src/utils/Config.h"

#define SCREENSHOT 0
#define SOCIAL_DATA 1
#define WHOIS 2

/*
 * for now only raw results
 */
JsonWhois::JsonWhois(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_JSONWHOIS;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &JsonWhois::replyFinishedRawJson);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_JSONWHOIS).toString();
}
JsonWhois::~JsonWhois(){
    delete manager;
}

void JsonWhois::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", "Token token="+m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case SCREENSHOT:
            url.setUrl("https://jsonwhois/api/v1/screenshot?domain="+target);
            break;
        case SOCIAL_DATA:
            url.setUrl("https://jsonwhois/api/v1/social?domain="+target);
            break;
        case WHOIS:
            url.setUrl("https://jsonwhois/api/v1/whois?domain="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}

