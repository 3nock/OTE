#include "SpamHaus.h"
#include "src/utils/Config.h"

#define FORWARD_SEARCH 0
#define REVERSE_SEARCH 1

/*
 * for now only raw results
 */
SpamHaus::SpamHaus(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_SPAMHAUS;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SpamHaus::replyFinishedRawJson);

    /* getting api-key */
    m_key = APIKEY.value(OSINT_MODULE_SPAMHAUS).toString();
}
SpamHaus::~SpamHaus(){
    delete manager;
}

void SpamHaus::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "Bearer "+m_key.toUtf8());

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case FORWARD_SEARCH:
            url.setUrl("https://api-pdns.spamhaustech.com/v2/_search/rrset/"+target);
            break;
        case REVERSE_SEARCH:
            url.setUrl("https://api-pdns.spamhaustech.com/v2/_search/rdata/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
    }
}
