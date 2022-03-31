#include "AbuseIPDB.h"
#include "src/utils/Config.h"

#define BLACKLIST_ENDPOINT 0
#define CHECK_ENDPOINT 1
#define CHECK_BLOCK_ENDPOINT 2

/*
 * 1,000 per day for free-user...
 * url-encode the data, maxAgeInDays...
 */
AbuseIPDB::AbuseIPDB(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_ABUSEIPDB;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &AbuseIPDB::replyFinishedRawJson);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_ABUSEIPDB).toString();
}
AbuseIPDB::~AbuseIPDB(){
    delete manager;
}

void AbuseIPDB::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Key", m_key.toUtf8());
    request.setRawHeader("Accept", "application/json");
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case BLACKLIST_ENDPOINT:
            url.setUrl("https://api.abuseipdb.com/api/v2/blacklist");
            break;
        case CHECK_ENDPOINT:
            url.setUrl("https://api.abuseipdb.com/api/v2/check?ipAddress="+target);
            url.toEncoded();
            break;
        case CHECK_BLOCK_ENDPOINT:
            url.setUrl("https://api.abuseipdb.com/api/v2/check-block?network="+target);
            url.toEncoded();
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }
}
