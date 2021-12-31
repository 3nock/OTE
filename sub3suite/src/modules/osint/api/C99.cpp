#include "C99.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define ALEXA_RANK 0
#define DISPOSABLE_MAIL_CHECK 1
#define DOMAIN_CHECKER 2
#define DOMAIN_HISTORY_CHECKER 3
#define EMAIL_VALIDATOR 4
#define WAF_CHECKER 5
#define GEOIP 6
#define HOST_TO_IP 7
#define IP_2_DOMAINS 8
#define IP_LOGGER 9
#define IP_VALIDATOR 10
#define IP_2_HOST 11
#define IP_2_SKYPE 12
#define LINK_BACKUP 13
#define MULTIPLE_PORT_SCANNER 14
#define NMAP_SCANNER 15
#define PHONE_LOOKUP 16
#define PING 17
#define PROXY_DETECTOR 18
#define SCREENSHOT_TOOL 19
#define SITE_REPUTATION_CHECKER 20
#define SKYPE_RESOLVER 21
#define SUBDOMAIN_FINDER 22
#define TOR_CHECKER 23
#define WEBSITE_HEADERS 24
#define WEBSITE_UP_OR_DOWN_CHECKER 25
#define WHOIS_CHECKER 26


/*
 * Others not yet implemented...
 */
C99::C99(ScanArgs args):
    AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = OSINT_MODULE_C99;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &C99::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &C99::replyFinishedSubdomain);

    /* getting api key... */
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value(OSINT_MODULE_C99).toString();
    Config::generalConfig().endGroup();
}
C99::~C99(){
    delete manager;
}

void C99::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ALEXA_RANK:
            url.setUrl("https://api.c99.nl/alexarank?key="+m_key+"&url="+target+"&json");
            break;
        case DISPOSABLE_MAIL_CHECK:
            url.setUrl("https://api.c99.nl/disposablemailchecker?key="+m_key+"&email="+target+"&json");
            break;
        case DOMAIN_CHECKER:
            url.setUrl("https://api.c99.nl/domainchecker?key="+m_key+"&domain="+target+"&json");
            break;
        case DOMAIN_HISTORY_CHECKER:
            url.setUrl("https://api.c99.nl/domainhistory?key="+m_key+"&domain="+target+"&json");
            break;
        case EMAIL_VALIDATOR:
            url.setUrl("https://api.c99.nl/emailvalidator?key="+m_key+"&email="+target+"&json");
            break;
        case WAF_CHECKER:
            url.setUrl("https://api.c99.nl/firewsubdomainIpdetector?key="+m_key+"&url="+target+"&json");
            break;
        case GEOIP:
            url.setUrl("https://api.c99.nl/geoip?key="+m_key+"&host="+target+"&json");
            break;
        case HOST_TO_IP:
            url.setUrl("https://api.c99.nl/dnsresolver?key="+m_key+"&host="+target+"&json");
            break;
        case IP_2_DOMAINS:
            url.setUrl("https://api.c99.nl/ip2domains?key="+m_key+"&ip="+target+"&json");
            break;
        case IP_LOGGER:
            url.setUrl("https://api.c99.nl/iplogger?key="+m_key+"&action=viewloggers&json");
            break;
        case IP_VALIDATOR:
            url.setUrl("https://api.c99.nl/ipvalidator?key="+m_key+"&ip="+target+"&json");
            break;
        case IP_2_HOST:
            url.setUrl("https://api.c99.nl/gethostname?key="+m_key+"&host="+target+"&json");
            break;
        case IP_2_SKYPE:
            url.setUrl("https://api.c99.nl/ip2skype?key="+m_key+"&ip="+target+"&json");
            break;
        case LINK_BACKUP:
            url.setUrl("https://api.c99.nl/linkbackup?key="+m_key+"&url="+target+"&json");
            break;
        case MULTIPLE_PORT_SCANNER:
            url.setUrl("https://api.c99.nl/portscanner?key="+m_key+"&host="+target+"&json");
            break;
        case NMAP_SCANNER:
            url.setUrl("https://api.c99.nl/nmap?key="+m_key+"&host="+target+"&json");
            break;
        case PHONE_LOOKUP:
            url.setUrl("https://api.c99.nl/phonelookup?key="+m_key+"&number="+target+"&json");
            break;
        case PING:
            url.setUrl("https://api.c99.nl/ping?key="+m_key+"&host="+target+"&json");
            break;
        case PROXY_DETECTOR:
            url.setUrl("https://api.c99.nl/proxydetector?key="+m_key+"&ip="+target+"&json");
            break;
        case SCREENSHOT_TOOL:
            url.setUrl("https://api.c99.nl/createscreenshot?key="+m_key+"&url="+target+"&json");
            break;
        case SITE_REPUTATION_CHECKER:
            url.setUrl("https://api.c99.nl/reputationchecker?key="+m_key+"&url="+target+"&json");
            break;
        case SKYPE_RESOLVER:
            url.setUrl("https://api.c99.nl/skyperesolver?key="+m_key+"&username="+target+"&json");
            break;
        case SUBDOMAIN_FINDER:
            url.setUrl("https://api.c99.nl/subdomainfinder?key="+m_key+"&domain="+target+"&json");
            break;
        case TOR_CHECKER:
            url.setUrl("https://api.c99.nl/torchecker?key="+m_key+"&ip="+target+"&json");
            break;
        case WEBSITE_HEADERS:
            url.setUrl("https://api.c99.nl/getheaders?key="+m_key+"&host="+target+"&json");
            break;
        case WEBSITE_UP_OR_DOWN_CHECKER:
            url.setUrl("https://api.c99.nl/upordown?key="+m_key+"&host="+target+"&json");
            break;
        case WHOIS_CHECKER:
            url.setUrl("https://api.c99.nl/whois?key="+m_key+"&domain="+target+"&json");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args.inputDomain){
        if(args.outputSubdomain){
            request.setUrl(url);
            request.setAttribute(QNetworkRequest::User, SUBDOMAIN_FINDER);
            manager->get(request);
            activeRequests++;
        }
    }
}

void C99::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = document.object();

    if(QUERY_TYPE == SUBDOMAIN_FINDER){
        if(jsonObject["success"].toBool())
        {
            foreach(const QJsonValue &value, jsonObject["subdomains"].toArray()){
                emit resultSubdomain(value.toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}
