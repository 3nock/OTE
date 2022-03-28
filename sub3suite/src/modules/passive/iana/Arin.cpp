#include "Arin.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASN_POCS 0
#define NET_CHILDREN 1
#define NET_PARENTS 2
#define NET_POCS 3
#define NET_RDNS 4
#define ORG_ASNS 5
#define ORG_NETS 6
#define ORG_POCS 7
#define POC_ASNS 8
#define POC_NETS 9
#define POC_ORGS 10
#define RDNS_NETS 11
#define REGISTRY_ASN 12
#define REGISTRY_IP 13
#define WHOIS_ASN 14
#define WHOIS_CUSTOMER 15
#define WHOIS_NET 16
#define WHOIS_ORG 17
#define WHOIS_POC 18
#define WHOIS_RDNS 19


/*
 * not well implemented yet for osint, only raw...
 */
Arin::Arin(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_ARIN;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Arin::replyFinishedRawJson);
}
Arin::~Arin(){
    delete manager;
}

void Arin::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");

    QUrl url;
    if(args.output_Raw){
        switch (args.raw_query_id) {
        case REGISTRY_IP:
            url.setUrl("https://rdap.arin.net/registry/ip/"+target);
            break;
        case REGISTRY_ASN:
            url.setUrl("https://rdap.arin.net/registry/autnum/"+target);
            break;
        case WHOIS_ASN:
            url.setUrl("http://whois.arin.net/rest/asn/"+target);
            break;
        case WHOIS_NET:
            url.setUrl("http://whois.arin.net/rest/net/"+target);
            break;
        case WHOIS_ORG:
            url.setUrl("http://whois.arin.net/rest/asn/"+target);
            break;
        case WHOIS_POC:
            url.setUrl("http://whois.arin.net/rest/poc/"+target);
            break;
        case WHOIS_RDNS:
            url.setUrl("http://whois.arin.net/rest/rdns/"+target);
            break;
        case WHOIS_CUSTOMER:
            url.setUrl("http://whois.arin.net/rest/customer/"+target);
            break;
        case POC_ASNS:
            url.setUrl("http://whois.arin.net/rest/poc/"+target+"/asns");
            break;
        case POC_NETS:
            url.setUrl("http://whois.arin.net/rest/poc/"+target+"/nets");
            break;
        case POC_ORGS:
            url.setUrl("http://whois.arin.net/rest/poc/"+target+"/orgs");
            break;
        case ORG_ASNS:
            url.setUrl("http://whois.arin.net/rest/org/"+target+"/asns");
            break;
        case ORG_NETS:
            url.setUrl("http://whois.arin.net/rest/org/"+target+"/nets");
            break;
        case ORG_POCS:
            url.setUrl("http://whois.arin.net/rest/org/"+target+"/pocs");
            break;
        case ASN_POCS:
            url.setUrl("http://whois.arin.net/rest/asn/"+target+"/pocs");
            break;
        case NET_POCS:
            url.setUrl("http://whois.arin.net/rest/net/"+target+"/pocs");
            break;
        case NET_RDNS:
            url.setUrl("http://whois.arin.net/rest/net/"+target+"/rdns");
            break;
        case NET_PARENTS:
            url.setUrl("http://whois.arin.net/rest/net/"+target+"/parent");
            break;
        case NET_CHILDREN:
            url.setUrl("http://whois.arin.net/rest/poc/"+target+"/children");
            break;
        case RDNS_NETS:
            url.setUrl("http://whois.arin.net/rest/rdns/"+target+"/nets");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
