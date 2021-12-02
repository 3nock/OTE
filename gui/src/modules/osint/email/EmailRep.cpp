#include "EmailRep.h"
#include "src/utils/Config.h"

#define EMAIL_QUERY 0


EmailRep::EmailRep(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "EmailRep";

    if(args->outputRaw)
        connect(manager, &MyNetworkAccessManager::finished, this, &EmailRep::replyFinishedRawJson);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("emailrep").toString();
    Config::generalConfig().endGroup();
}
EmailRep::~EmailRep(){
    delete manager;
}

void EmailRep::start(){
    QNetworkRequest request;
    request.setRawHeader("Key", m_key.toUtf8());
    // request.setRawHeader("User-Agent", userAget);

    QUrl url;
    if(args->outputRaw){
        switch(args->rawOption){
        case EMAIL_QUERY:
            url.setUrl("https://emailrep.io/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

