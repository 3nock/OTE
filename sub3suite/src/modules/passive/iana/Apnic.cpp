#include "Apnic.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define IP 0

/*
 * not well implemented yet for osint, only raw...
 */
Apnic::Apnic(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_APNIC;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Apnic::replyFinishedRawJson);
}
Apnic::~Apnic(){
    delete manager;
}

void Apnic::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case IP:
            url.setUrl("http://rdap.apnic.net/ip/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
