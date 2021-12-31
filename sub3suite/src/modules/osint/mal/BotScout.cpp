#include "BotScout.h"

#define ALL 0
#define EMAIL 1
#define IP 2
#define NAME 3

/*
 * 100 per day for free-user...
 * has subscription module for more than 100 queries a day...
 */
BotScout::BotScout(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "BotScout";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &BotScout::replyFinishedRawJson);
}
BotScout::~BotScout(){
    delete manager;
}

void BotScout::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ALL:
            url.setUrl("http://botscout.com/test/?all="+target);
            break;
        case EMAIL:
            url.setUrl("http://botscout.com/test/?mail="+target);
            break;
        case IP:
            url.setUrl("http://botscout.com/test/?ip="+target);
            break;
        case NAME:
            url.setUrl("http://botscout.com/test/?name="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

