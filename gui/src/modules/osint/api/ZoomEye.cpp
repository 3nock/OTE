#include "ZoomEye.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DOMAINS 0

/*
 * produces a vast amount of information...
 * subdomain-ip-asn-org-geoLoc
 */
ZoomEye::ZoomEye(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "ZoomEye";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &ZoomEye::replyFinishedRaw);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &ZoomEye::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &ZoomEye::replyFinishedSubdomain);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &ZoomEye::replyFinishedAsn);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &ZoomEye::replyFinishedIp);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("zoomeye").toString();
    Config::generalConfig().endGroup();
}
ZoomEye::~ZoomEye(){
    delete manager;
}

void ZoomEye::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("API-KEY", m_key.toUtf8());

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case DOMAINS:
            url.setUrl("https://api.zoomeye.org/host/search?query=hostname:*."+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://api.zoomeye.org/host/search?query=hostname:*."+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void ZoomEye::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(document.object()["total"].toInt()){
        QJsonArray matches = document.object()["matches"].toArray();
        foreach(const QJsonValue &value, matches){
            emit subdomain(value.toObject()["rdns"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void ZoomEye::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(document.object()["total"].toInt()){
        QJsonArray matches = document.object()["matches"].toArray();
        foreach(const QJsonValue &value, matches){
            emit ip(value.toObject()["ip"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void ZoomEye::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(document.object()["total"].toInt()){
        QJsonArray matches = document.object()["matches"].toArray();
        foreach(const QJsonValue &value, matches){
            QString ASN = value.toObject()["geoinfo"].toObject()["asn"].toString();
            QString org = value.toObject()["geoinfo"].toObject()["organization"].toString();
            emit asn(ASN, org);
            log.resultsCount++;
        }
    }

    end(reply);
}

void ZoomEye::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if(document.object()["total"].toInt()){
        QJsonArray matches = document.object()["matches"].toArray();
        foreach(const QJsonValue &value, matches){
            emit subdomain(value.toObject()["rdns"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

/*
 * QUERY
{
"total": 2080,
"available": 2080,
"matches":[
    {"rdns": "imap.google.com",
    "jarm": "",
    "ico": {"mmh3": "", "md5": ""},
    "txtfile": {"robotsmd5": "", "securitymd5": ""},
    "ip": "161.42.161.43",
    "portinfo": {
        "hostname": "",
        "os": "",
        "port": 7443,
        "service": "https",
        "title": null,
        "version": "",
        "device": "",
        "extrainfo": "",
        "rdns": "imap.google.com",
        "app": "Apache httpd",
        "banner": "HTTP/1.1  400 Bad Request\nSec-Websocket-Version: 13\n\n"
    },
    "ssl": "...the certificate..."
    "timestamp": "2021-09-28T13:27:05",
    "geoinfo": {
        "continent":{"code": "AF",
            "names": {"en": "Africa", "zh-CN": "\u975e\u6d32"},
            "geoname_id": null
        },
        "country": {
            "code": "TZ",
            "names": {"en": "Tanzania", "zh-CN": "\u5766\u6851\u5c3c\u4e9a"},
            "geoname_id": null
        },
        "base_station": "",
        "city": {
            "names": {"en": "", "cn": "", "zh-CN": ""},
            "geoname_id": null
        },
        "isp": "",
        "organization": "Google",
        "idc": "",
        "location": {"lon": "24.586189", "lat": "-5.86621"},
        "aso": null,
        "asn": "21246",
        "subdivisions": {"names": {"en": "Unknown", "zh-CN": "Unknown"}, "geoname_id": null},
        "PoweredBy": "IPIP",
        "organization_CN": null
    },
    "protocol": {
        "application": "HTTP",
        "probe": "GetRequestHost",
        "transport": "tcp"},
        "honeypot": 0
    },
    {...others...}
    ]
}

YOU CAN SEARCH:
app	      string	application\software\product and etc.)	app: ProFTPD
ver	      string	versions                                ver:2.1
device	  string	device type                             device:router
os	      string	operating system                        os:windows
service	  string	service                                 service:http
ip	      string	ip address                              ip:192.168.1.1
cidr	  string	CIDR Address prefix                     cidr:192.168.1.1/24
hostname  string	hostname                                hostname:google.com
port	  string	port number                             port:80
city	  string	city name                               city:beijing
country	  string	country name                            country:china
asn       integer	asn number                              asn:8978

*/
