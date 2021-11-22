#include "TruMail.h"

#define EMAIL 0


TruMail::TruMail(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "TruMail";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &TruMail::replyFinishedRaw);
}
TruMail::~TruMail(){
    delete manager;
}

void TruMail::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case EMAIL:
            url.setUrl("https://api.trumail.io/v2/lookups/json?email="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
