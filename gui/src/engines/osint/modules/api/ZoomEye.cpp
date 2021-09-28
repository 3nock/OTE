#include "ZoomEye.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


/*
 * produces a vast amount of information...
 * subdomain-ip-asn-org-geoLoc
 */
ZoomEye::ZoomEye(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ZoomEye::replyFinished);
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
    QUrl url("https://api.zoomeye.org/host/search?query=hostname:*."+target);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("API-KEY", m_key.toUtf8());
    manager->get(request);
}

void ZoomEye::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        if(jsonReply.object()["total"].toInt()){
            QJsonArray matches = jsonReply.object()["matches"].toArray();
            foreach(const QJsonValue &value, matches)
                emit scanResults(value.toObject()["rdns"].toString());
            /*
                IP
            foreach(const QJsonValue &value, matches)
                emit scanResults(value.toObject()["ip"].toString());

                ASN & ORGANIZATION
            foreach(const QJsonValue &value, matches){
                QString asn = value.toObject()["geoinfo"].toObject()["asn"].toString();
                QString org = value.toObject()["geoinfo"].toObject()["organization"].toString();
            }
            */
        }
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
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
