#include "ZETAlytics.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


ZETAlytics::ZETAlytics(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_ZETALYTICS;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ZETAlytics::replyFinishedRawJson);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_ZETALYTICS).toString();
}
ZETAlytics::~ZETAlytics(){
    delete manager;
}

void ZETAlytics::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args.outputRaw){
        url.setUrl("https://zonecruncher.com/api/v1/subdomains?q="+target+"&token="+m_key);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
