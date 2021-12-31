#include "LeakIX.h"
#include "src/utils/Config.h"

#define HOST 0


LeakIX::LeakIX(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "LeakIX";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &LeakIX::replyFinishedRawJson);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("leakix").toString();
    Config::generalConfig().endGroup();
}
LeakIX::~LeakIX(){
    delete manager;
}

void LeakIX::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("api-key", m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case HOST:
            url.setUrl("https://leakix.net/host/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
