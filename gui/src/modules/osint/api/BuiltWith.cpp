#include "BuiltWith.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define LOOKUP 0

/*
 * for now only raw results...
 * for free api, 1 request per second...
 */
BuiltWith::BuiltWith(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "BuiltWith";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &BuiltWith::replyFinishedRaw);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("builtwith").toString();
    Config::generalConfig().endGroup();
}
BuiltWith::~BuiltWith(){
    delete manager;
}

void BuiltWith::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case LOOKUP:
            url.setUrl("https://api.builtwith.com/free1/api.json?KEY="+m_key+"&LOOKUP="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /*
     * Others not implemented yet...
     */
}
