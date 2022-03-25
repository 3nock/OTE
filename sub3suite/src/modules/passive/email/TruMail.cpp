#include "TruMail.h"

#define EMAIL 0


TruMail::TruMail(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "TruMail";

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &TruMail::replyFinishedRawJson);
}
TruMail::~TruMail(){
    delete manager;
}

void TruMail::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch(args.raw_query_id){
        case EMAIL:
            url.setUrl("https://api.trumail.io/v2/lookups/json?email="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
