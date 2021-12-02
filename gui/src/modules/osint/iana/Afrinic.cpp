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
Afrinic::Afrinic(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Afrinic";

    if(args->outputRaw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Afrinic::replyFinishedRawJson);
}
Afrinic::~Afrinic(){
    delete manager;
}

void Afrinic::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case AS_NUMBERS:
            url.setUrl("https://rdap.afrinic.net/rdap/autnum/"+args->target);
            break;
        case ENTITY:
            url.setUrl("https://rdap.afrinic.net/rdap/entity/"+args->target);
            break;
        case IP_NETWORKS:
            url.setUrl("https://rdap.afrinic.net/rdap/ip/"+args->target);
            break;
        case RDNS:
            url.setUrl("https://rdap.afrinic.net/rdap/domain/"+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
