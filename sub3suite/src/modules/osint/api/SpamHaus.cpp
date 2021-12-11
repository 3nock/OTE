#include "SpamHaus.h"
#include "src/utils/Config.h"

#define FORWARD_SEARCH 0
#define REVERSE_SEARCH 1

SpamHaus::SpamHaus(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "SpamHaus";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &SpamHaus::replyFinishedRawJson);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("spamhaus").toString();
    Config::generalConfig().endGroup();
}
SpamHaus::~SpamHaus(){
    delete manager;
}

void SpamHaus::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "Bearer "+m_key.toUtf8());

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case FORWARD_SEARCH:
            url.setUrl("https://api-pdns.spamhaustech.com/v2/_search/rrset/"+args->target);
            break;
        case REVERSE_SEARCH:
            url.setUrl("https://api-pdns.spamhaustech.com/v2/_search/rdata/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
