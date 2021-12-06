#include "Ripe.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ABUSE_CONTACT_FINDER 0
#define ADDRESS_SPACE_HIERARCHY 1
#define ADDRESS_SPACE_USAGE 2
#define AS_OVERVIEW 3
#define AS_PATH_LENGTH 4
#define AS_ROOTING_CONSISTENCY 5
#define AS_NEIGHBOURS 6
#define AS_NEIGHBOURS_HISTORY 7
#define BLOCKLIST 8
#define COUNTRY_ASNS 9
#define COUNTRY_RESOURCE_STATS 10
#define COUNTRY_RESOURCE_LISTS 11
#define DNS_CHAIN 12
#define HISTORICAL_WHOIS 13
#define IANA_REGISTRY_INFO 14
#define LOOKING_GLASS 15
#define MAXMIND_GEO_LITE 16
#define MAXMIND_GEO_LITE_AS 17
#define NETWORK_INFO 18
#define PREFIX_OVERVIEW 19
#define PREFIX_ROUTING_CONSISTENCY 20
#define RIR 21
#define RIR_STATS_COUNTRY 22
#define RIR_PREFIX_SIZE_DISTRIBUTION 23
#define RIS_FIRST_LAST_SEEN 24
#define RIS_PREFIXES 25
#define RIS_PEERINGS 26
#define RELATED_PREFIXES 267
#define REVERSE_DNS 28
#define REVERSE_DNS_IP 29
#define ROUTING_HISTORY 30
#define ROUTING_STATUS 31
#define RRC_INFO 32
#define VISIBILITY 33
#define WHATS_MY_IP 34
#define WHOIS 35

/*
 * has a dns-lookup
 * has prefix overview
 * has RIS Peering
 * has related prefixes
 * has dns reverse
 * has visibility
 */
Ripe::Ripe(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Ripe";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Ripe::replyFinishedRawJson);
    if(args->outputAsn)
        connect(manager, &NetworkAccessManager::finished, this, &Ripe::replyFinishedAsn);
    if(args->outputCidr)
        connect(manager, &NetworkAccessManager::finished, this, &Ripe::replyFinishedCidr);
}
Ripe::~Ripe(){
    delete manager;
}

void Ripe::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case ABUSE_CONTACT_FINDER:
            url.setUrl("https://stat.ripe.net/data/abuse-contact-finder/data.json?resource="+args->target);
            break;
        case ADDRESS_SPACE_HIERARCHY:
            url.setUrl("https://stat.ripe.net/data/address-space-hierarchy/data.json?resource="+args->target);
            break;
        case ADDRESS_SPACE_USAGE:
            url.setUrl("https://stat.ripe.net/data/address-space-usage/data.json?resource="+args->target);
            break;
        case AS_OVERVIEW:
            url.setUrl("https://stat.ripe.net/data/as-overview/data.json?resource="+args->target);
            break;
        case AS_PATH_LENGTH:
            url.setUrl("https://stat.ripe.net/data/as-path-length/data.json?resource="+args->target);
            break;
        case AS_ROOTING_CONSISTENCY:
            url.setUrl("https://stat.ripe.net/data/as-routing-consistency/data.json?resource="+args->target);
            break;
        case AS_NEIGHBOURS:
            url.setUrl("https://stat.ripe.net/data/asn-neighbours/data.json?resource="+args->target);
            break;
        case AS_NEIGHBOURS_HISTORY:
            url.setUrl("https://stat.ripe.net/data/asn-neighbours-history/data.json?resource="+args->target);
            break;
        case BLOCKLIST:
            url.setUrl("https://stat.ripe.net/data/blocklist/data.json?resource="+args->target);
            break;
        case COUNTRY_ASNS:
            url.setUrl("https://stat.ripe.net/data/country-asns/data.json?resource="+args->target+"&lod=1");
            break;
        case COUNTRY_RESOURCE_STATS:
            url.setUrl("https://stat.ripe.net/data/country-resource-stats/data.json?resource="+args->target);
            break;
        case COUNTRY_RESOURCE_LISTS:
            url.setUrl(" https://stat.ripe.net/data/country-resource-list/data.json?resource="+args->target);
            break;
        case DNS_CHAIN:
            url.setUrl("https://stat.ripe.net/data/dns-chain/data.json?resource="+args->target);
            break;
        case HISTORICAL_WHOIS:
            url.setUrl("https://stat.ripe.net/data/historical-whois/data.json?resource="+args->target);
            break;
        case IANA_REGISTRY_INFO:
            url.setUrl("https://stat.ripe.net/data/iana-registry-info/data.json?resource="+args->target);
            break;
        case LOOKING_GLASS:
            url.setUrl("https://stat.ripe.net/data/looking-glass/data.json?resource="+args->target);
            break;
        case MAXMIND_GEO_LITE:
            url.setUrl("https://stat.ripe.net/data/maxmind-geo-lite/data.json?resource="+args->target);
            break;
        case MAXMIND_GEO_LITE_AS:
            url.setUrl("https://stat.ripe.net/data/maxmind-geo-lite-announced-by-as/data.json?resource="+args->target);
            break;
        case NETWORK_INFO:
            url.setUrl("https://stat.ripe.net/data/network-info/data.json?resource="+args->target);
            break;
        case PREFIX_OVERVIEW:
            url.setUrl("https://stat.ripe.net/data/prefix-overview/data.json?resource="+args->target);
            break;
        case PREFIX_ROUTING_CONSISTENCY:
            url.setUrl("https://stat.ripe.net/data/prefix-routing-consistency/data.json?resource="+args->target);
            break;
        case RIR:
            url.setUrl("https://stat.ripe.net/data/rir/data.json?resource="+args->target+"&lod=2");
            break;
        case RIR_STATS_COUNTRY:
            url.setUrl("https://stat.ripe.net/data/rir-stats-country/data.json?resource="+args->target);
            break;
        case RIR_PREFIX_SIZE_DISTRIBUTION:
            url.setUrl("https://stat.ripe.net/data/rir-prefix-size-distribution/data.json?resource="+args->target);
            break;
        case RIS_FIRST_LAST_SEEN:
            url.setUrl("https://stat.ripe.net/data/ris-first-last-seen/data.json?resource="+args->target);
            break;
        case RIS_PREFIXES:
            url.setUrl("https://stat.ripe.net/data/ris-prefixes/data.json?resource="+args->target+"&list_prefixes=true");
            break;
        case RIS_PEERINGS:
            url.setUrl("https://stat.ripe.net/data/ris-peerings/data.json?resource="+args->target);
            break;
        case RELATED_PREFIXES:
            url.setUrl("https://stat.ripe.net/data/related-prefixes/data.json?resource="+args->target);
            break;
        case REVERSE_DNS:
            url.setUrl("https://stat.ripe.net/data/reverse-dns/data.json?resource="+args->target);
            break;
        case REVERSE_DNS_IP:
            url.setUrl("https://stat.ripe.net/data/reverse-dns-ip/data.json?resource="+args->target);
            break;
        case ROUTING_HISTORY:
            url.setUrl("https://stat.ripe.net/data/routing-history/data.json?resource="+args->target);
            break;
        case ROUTING_STATUS:
            url.setUrl("https://stat.ripe.net/data/routing-status/data.json?resource="+args->target);
            break;
        case RRC_INFO:
            url.setUrl("https://stat.ripe.net/data/rrc-info/data.json?resource="+args->target);
            break;
        case VISIBILITY:
            url.setUrl(" https://stat.ripe.net/data/visibility/data.json?resource="+args->target);
            break;
        case WHATS_MY_IP:
            url.setUrl("https://stat.ripe.net/data/whats-my-ip/data.json?resource="+args->target);
            break;
        case WHOIS:
            url.setUrl("https://stat.ripe.net/data/whois/data.json?resource="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputIp){
        if(args->outputAsn){
            url.setUrl("https://stat.ripe.net/data/network-info/data.json?resource="+args->target);
            request.setAttribute(QNetworkRequest::User, NETWORK_INFO);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args->inputAsn){
        if(args->outputCidr){
            url.setUrl("https://stat.ripe.net/data/ris-prefixes/data.json?resource="+args->target+"&list_prefixes=true");
            request.setAttribute(QNetworkRequest::User, RIS_PREFIXES);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Ripe::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == NETWORK_INFO){
        QJsonArray asns = document.object()["data"].toObject()["asns"].toArray();
        foreach(const QJsonValue &value, asns){
            emit asn(value.toString(), "");
            log.resultsCount++;
        }
    }

    end(reply);
}

void Ripe::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(requestType == RIS_PREFIXES){
        QJsonObject prefixes = document.object()["data"].toObject()["prefixes"].toObject();

        /* for ipv4 */
        QJsonArray v4_originating = prefixes["v4"].toObject()["originating"].toArray();
        foreach(const QJsonValue &value, v4_originating){
            emit cidr(value.toString());
            log.resultsCount++;
        }
        QJsonArray v4_transiting = prefixes["v4"].toObject()["transiting"].toArray();
        foreach(const QJsonValue &value, v4_transiting){
            emit cidr(value.toString());
            log.resultsCount++;
        }
        /* for ipv6 */
        QJsonArray v6_originating = prefixes["v6"].toObject()["originating"].toArray();
        foreach(const QJsonValue &value, v4_originating){
            emit cidr(value.toString());
            log.resultsCount++;
        }
        QJsonArray v6_transiting = prefixes["v6"].toObject()["transiting"].toArray();
        foreach(const QJsonValue &value, v4_transiting){
            emit cidr(value.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}
