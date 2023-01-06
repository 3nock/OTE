#include "EmailRep.h"
#include "src/utils/Config.h"

#define EMAIL_QUERY 0


EmailRep::EmailRep(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_EMAILREP;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &EmailRep::replyFinishedRawJson);

    /* getting api-key */
    m_key = gConfig.keys.EmailRep;
}
EmailRep::~EmailRep(){
    delete manager;
}

void EmailRep::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Key", m_key.toUtf8());
    // request.setRawHeader("User-Agent", userAget);
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case EMAIL_QUERY:
            url.setUrl("https://emailrep.io/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
    }
}

