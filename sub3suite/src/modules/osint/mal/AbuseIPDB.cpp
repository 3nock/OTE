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
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "AbuseIPDB";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &AbuseIPDB::replyFinishedRawJson);
    ///
    /// get api key...
    ///
    
    m_key = APIKEY.value("abuseipdb").toString();
    
}
AbuseIPDB::~AbuseIPDB(){
    delete manager;
}

void AbuseIPDB::start(){
    QNetworkRequest request;
    request.setRawHeader("Key", m_key.toUtf8());
    request.setRawHeader("Accept", "application/json");

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
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
        activeRequests++;
    }
}
