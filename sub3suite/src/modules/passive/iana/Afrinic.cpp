#include "Afrinic.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define AS_NUMBERS 0
#define ENTITY 1
#define IP_NETWORKS 2
#define RDNS 3

/*
 * not well implemented yet for osint, only raw...
 */
Afrinic::Afrinic(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_AFRINIC;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Afrinic::replyFinishedRawJson);
}
Afrinic::~Afrinic(){
    delete manager;
}

void Afrinic::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case AS_NUMBERS:
            url.setUrl("https://rdap.afrinic.net/rdap/autnum/"+target);
            break;
        case ENTITY:
            url.setUrl("https://rdap.afrinic.net/rdap/entity/"+target);
            break;
        case IP_NETWORKS:
            url.setUrl("https://rdap.afrinic.net/rdap/ip/"+target);
            break;
        case RDNS:
            url.setUrl("https://rdap.afrinic.net/rdap/domain/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
