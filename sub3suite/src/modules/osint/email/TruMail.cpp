#include "TruMail.h"

#define EMAIL 0


TruMail::TruMail(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "TruMail";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &TruMail::replyFinishedRawJson);
}
TruMail::~TruMail(){
    delete manager;
}

void TruMail::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch(args.rawOption){
        case EMAIL:
            url.setUrl("https://api.trumail.io/v2/lookups/json?email="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
