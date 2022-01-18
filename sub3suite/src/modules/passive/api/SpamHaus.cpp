#include "SpamHaus.h"
#include "src/utils/Config.h"

#define FORWARD_SEARCH 0
#define REVERSE_SEARCH 1

SpamHaus::SpamHaus(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "SpamHaus";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &SpamHaus::replyFinishedRawJson);
    ///
    /// getting api-key...
    ///
    
    m_key = APIKEY.value("spamhaus").toString();
    
}
SpamHaus::~SpamHaus(){
    delete manager;
}

void SpamHaus::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "Bearer "+m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case FORWARD_SEARCH:
            url.setUrl("https://api-pdns.spamhaustech.com/v2/_search/rrset/"+target);
            break;
        case REVERSE_SEARCH:
            url.setUrl("https://api-pdns.spamhaustech.com/v2/_search/rdata/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}