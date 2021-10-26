#include "ViewDns.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ABUSE_CONTACT_LOOKUP 0
#define DNS_PROPAGATION_CHECKER 1
#define DNS_RECORD_LOOKUP 2
#define DOMAIN_IP_WHOIS 3
#define IP_HISTORY 4
#define IP_LOCATION_FINDER 5
#define MAC_ADDRESS_LOOKUP 6
#define PING 7
#define PORT_SCANNER 8
#define REVERSE_DNS_LOOKUP 9
#define REVERSE_IP_LOOKUP 10
#define REVERSE_MX_LOOKUP 11
#define REVERSE_NS_LOOKUP 12
#define REVERSE_WHOIS_LOOKUP 13

/*
 * only 250 api csubdomainIps available for free...
 */
ViewDns::ViewDns(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "ViewDns";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &ViewDns::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &ViewDns::replyFinishedSubdomain);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("viewdns").toString();
    Config::generalConfig().endGroup();
}
ViewDns::~ViewDns(){
    delete manager;
}

void ViewDns::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case ABUSE_CONTACT_LOOKUP:
            url.setUrl("https://api.viewdns.info/abuselookup/?domain="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case DNS_PROPAGATION_CHECKER:
            url.setUrl("https://api.viewdns.info/propagation/?domain="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case DNS_RECORD_LOOKUP:
            url.setUrl("https://api.viewdns.info/dnsrecord/?domain="+args->target+"&recordtype=ANY&apikey="+m_key+"&output=json");
            break;
        case DOMAIN_IP_WHOIS:
            url.setUrl("https://api.viewdns.info/whois/?domain="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case IP_HISTORY:
            url.setUrl("https://api.viewdns.info/iphistory/?domain="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case IP_LOCATION_FINDER:
            url.setUrl("https://api.viewdns.info/iplocation/?ip="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case MAC_ADDRESS_LOOKUP:
            url.setUrl("https://api.viewdns.info/maclookup/?mac="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case PING:
            url.setUrl("https://api.viewdns.info/ping/?host="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case PORT_SCANNER:
            url.setUrl("https://api.viewdns.info/portscan/?host="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_DNS_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversedns/?ip="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_IP_LOOKUP:
            url.setUrl("https://api.viewdns.info/reverseip/?host="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_MX_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversemx/?mx="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_NS_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversens/?ns="+args->target+"&apikey="+m_key+"&output=json");
            break;
        case REVERSE_WHOIS_LOOKUP:
            url.setUrl("https://api.viewdns.info/reversewhois/?q="+args->target+"&apikey="+m_key+"&output=json");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    {
        /*
         * nothing yet...
         */
    }
}

void ViewDns::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }
    /*
     * Nothing yet...
     */
    end(reply);
}
