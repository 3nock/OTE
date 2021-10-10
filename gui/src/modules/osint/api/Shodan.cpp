#include "Shodan.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * 1 request/ second
 */
Shodan::Shodan(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Shodan::replyFinished);
    ///
    /// get api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("shodan").toString();
    Config::generalConfig().endGroup();
}
Shodan::~Shodan(){
    delete manager;
}

void Shodan::start(){
    QNetworkRequest request;
    QUrl url("https://api.shodan.io/dns/domain/"+args->target+"?key="+m_key);
    request.setUrl(url);
    manager->get(request);
}

void Shodan::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray data = jsonReply.object()["data"].toArray();
        QJsonArray subdomains = jsonReply.object()["subdomains"].toArray();
        ///
        /// for subdomains...
        ///
        foreach(const QJsonValue &value, data){
            QString type = value.toObject()["type"].toString();
            QString hostname = value.toObject()["value"].toString();
            if(type == "MX" || type == "NS" || type == "CNAME")
                emit scanResults(hostname);
        }
        foreach(const QJsonValue &value, subdomains)
            emit scanResults(value.toString());
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*
 * https://api.shodan.io/account/profile?key={YOUR_API_KEY}
{
    "member": true,
    "credits": 0,
    "display_name": null,
    "created": "2020-06-15T10:44:43.148000"
}

DNS

https://api.shodan.io/dns/domain/google.com?key={YOUR_API_KEY}
{
    "domain": "google.com",
    "tags": [
        "ipv6"
    ],
    "data": [
        {
            "subdomain": "",
            "type": "MX",
            "value": "aspmx.l.google.com",
            "last_seen": "2021-01-19T22:23:15.978799+00:00"
        },
        {
            "subdomain": "*.auth.corp",
            "type": "CNAME",
            "value": "uberproxy.l.google.com",
            "last_seen": "2021-01-26T13:04:34.018114+00:00"
        },
        {
            "subdomain": "*.cloud.sandbox",
            "type": "A",
            "value": "74.125.142.81",
            "last_seen": "2021-01-15T12:57:18.133727+00:00"
        },
        ...
    ],
    "subdomains": [
        "*.auth.corp",
        "*.cloud.sandbox",
        "*.composer-staging.cloud",
        ...
    ],
    "more": true
}

RESOLVE MULTIPLE

https://api.shodan.io/dns/resolve?hostnames=google.com,facebook.com&key={YOUR_API_KEY}
{
    "google.com": "172.217.6.46",
    "facebook.com": "157.240.22.35"
}

REVERSE MULTIPLE

https://api.shodan.io/dns/reverse?ips=8.8.8.8,1.1.1.1&key={YOUR_API_KEY}
{
    "8.8.8.8": [
        "dns.google"
    ],
    "1.1.1.1": [
        "one.one.one.one"
    ]
}

API Status

https://api.shodan.io/api-info?key={YOUR_API_KEY}
{
    "scan_credits": 100000,
    "usage_limits": {
        "scan_credits": -1,
        "query_credits": -1,
        "monitored_ips": -1
    },
    "plan": "stream-100",
    "https": false,
    "unlocked": true,
    "query_credits": 100000,
    "monitored_ips": 19,
    "unlocked_left": 100000,
    "telnet": false
}

Return All Servies Running on a host

https://api.shodan.io/shodan/host/8.8.8.8?key={YOUR_API_KEY}
{
    "region_code": null,
    "ip": 134744072,
    "postal_code": null,
    "country_code": "US",
    "city": null,
    "dma_code": null,
    "last_update": "2021-01-22T08:49:35.190817",
    "latitude": 37.751,
    "tags": [],
    "area_code": null,
    "country_name": "United States",
    "hostnames": [
        "dns.google"
    ],
    "org": "Google",
    "data": [
        {
            "_shodan": {
                "id": "cea5795b-55fd-4595-b9e5-ad5ca847cb4b",
                "options": {},
                "ptr": true,
                "module": "dns-udp",
                "crawler": "ac284849be0745621b3c518f74c14cf43cafbf08"
            },
            "hash": -553166942,
            "os": null,
            "opts": {
                "raw": "34ef818200010000000000000776657273696f6e0462696e640000100003"
            },
            "ip": 134744072,
            "isp": "Google",
            "port": 53,
            "hostnames": [
                "dns.google"
            ],
            "location": {
                "city": null,
                "region_code": null,
                "area_code": null,
                "longitude": -97.822,
                "country_code3": null,
                "country_name": "United States",
                "postal_code": null,
                "dma_code": null,
                "country_code": "US",
                "latitude": 37.751
            },
            "dns": {
                "resolver_hostname": null,
                "recursive": true,
                "resolver_id": null,
                "software": null
            },
            "timestamp": "2021-01-22T08:49:35.190817",
            "domains": [
                "dns.google"
            ],
            "org": "Google",
            "data": "\nRecursion: enabled",
            "asn": "AS15169",
            "transport": "udp",
            "ip_str": "8.8.8.8"
        }
    ],
    "asn": "AS15169",
    "isp": "Google",
    "longitude": -97.822,
    "country_code3": null,
    "domains": [
        "dns.google"
    ],
    "ip_str": "8.8.8.8",
    "os": null,
    "ports": [
        53
    ]
}
 */
