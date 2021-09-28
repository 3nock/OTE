#include "ThreatBook.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


ThreatBook::ThreatBook(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ThreatBook::replyFinished);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("threatbook").toString();
    Config::generalConfig().endGroup();
}
ThreatBook::~ThreatBook(){
    delete manager;
}

void ThreatBook::start(){
    QNetworkRequest request;
    QUrl url("https://api.threatbook.cn/v3/domain/sub_domains?apikey="+m_key+"&resource="+target);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void ThreatBook::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.object()["data"].toObject()["sub_domains"].toObject()["data"].toArray();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*
 * apikey=
 * resource=
 *
 * SUBDOMAINS
https://api.threatbook.cn/v3/domain/sub_domains
{
    "data": {
        "domain": "000000.life",
        "sub_domains": {
            "total": "12",
            "data": [
                "0.000000.life",
                "autodiscover.000000.life",
                "cpanel.0.000000.life",
                "cpanel.000000.life",
                "mail.0.000000.life",
                "mail.000000.life",
                "webdisk.0.000000.life",
                "webdisk.000000.life",
                "webmail.0.000000.life",
                "webmail.000000.life",
                "www.0.000000.life",
                "www.000000.life"
            ]
        }
    },
    "response_code": 0,
    "verbose_msg": "OK"
}

 * DOMAIN-QUERY
 *
https://api.threatbook.cn/v3/domain/query
{
    "data": {
        "bibme.org": {
            "samples": [],
            "tags_classes": [],
            "judgments": [
                "Whitelist"
            ],
            "intelligences": {
                "threatbook_lab": [
                    {
                        "source": "ThreatBook Labs",
                        "confidence": 100,
                        "expired": false,
                        "intel_tags": [],
                        "find_time": "2016-05-25 01:08:30",
                        "intel_types": [
                            "Whitelist"
                        ]
                    }
                ],
                "x_reward": [],
                "open_source": [
                    {
                        "source": "Open Source ",
                        "confidence": 50,
                        "expired": false,
                        "intel_tags": [],
                        "find_time": "2017-07-11 04:39:50",
                        "intel_types": [
                            "Whitelist"
                        ]
                    },
                    {
                        "source": "alexa.com",
                        "confidence": 25,
                        "expired": true,
                        "intel_tags": [],
                        "find_time": "2015-12-09 06:19:27",
                        "intel_types": [
                            "Whitelist"
                        ]
                    }
                ]
            },
            "cas": [],
            "rank": {
                "alexa_rank": {
                    "global_rank": 9384
                },
                "umbrella_rank": {
                    "global_rank": 363574
                }
            },
            "cur_whois": {
                "cdate": "2007-04-04 06:21:58",
                "edate": "2022-04-04 06:21:58",
                "udate": "2016-08-16 00:20:32",
                "alexa": "18671",
                "registrar_name": "MarkMonitor Inc.",
                "name_server": "NS-1101.AWSDNS-09.ORG|NS-228.AWSDNS-28.COM|NS-860.AWSDNS-43.NET|NS-1926.AWSDNS-48.CO.UK",
                "registrant_name": "",
                "registrant_email": "",
                "registrant_company": "Chegg Inc.",
                "registrant_address": "",
                "registrant_phone": ""
            },
            "cur_ips": [
                {
                    "ip": "54.192.151.53",
                    "carrier": "amazon.com",
                    "location": {
                        "country": "Singapore",
                        "province": "Singapore",
                        "city": "",
                        "lng": "103.82",
                        "lat": "1.352",
                        "country_code": "SG"
                    }
                },
                {
                    "ip": "54.192.151.58",
                    "carrier": "amazon.com",
                    "location": {
                        "country": "Singapore",
                        "province": "Singapore",
                        "city": "",
                        "lng": "103.82",
                        "lat": "1.352",
                        "country_code": "SG"
                    }
                }
            ]
        }
    },
    "response_code": 0,
    "verbose_msg": "OK"
}

 * IP-QUERY
 *
https://api.threatbook.cn/v3/ip/query
{
    "data": {
        "159.203.93.255": {
            "samples": [],
            "tags_classes": [],
            "judgments": [
                "IDC"
            ],
            "intelligences": {
                "threatbook_lab": [
                    {
                        "source": "ThreatBook Labs",
                        "confidence": 70,
                        "expired": false,
                        "intel_tags": [],
                        "find_time": "2016-05-17 20:18:33",
                        "intel_types": [
                            "IDC"
                        ],
                        "update_time": "2016-05-17 20:18:33"
                    },
                    {
                        "source": "ThreatBook Labs",
                        "confidence": 90,
                        "expired": true,
                        "intel_tags": [],
                        "find_time": "2018-05-04 17:56:17",
                        "intel_types": [
                            "IDC"
                        ],
                        "update_time": "2018-08-02 21:02:25"
                    }
                ],
                "x_reward": [],
                "open_source": []
            },
            "scene": "",
            "basic": {
                "carrier": "digitalocean.com",
                "location": {
                    "country": "United States",
                    "province": "New York",
                    "city": "New York City",
                    "lng": "-74.006",
                    "lat": "40.713",
                    "country_code": "US"
                }
            },
            "asn": {
                "rank": 2,
                "info": "DIGITALOCEAN-ASN - DigitalOcean, LLC, US",
                "number": 14061
            },
            "ports": [],
            "cas": [],
            "update_time": "2016-05-17 20:18:33",
            "rdns_list": [],
            "sum_cur_domains": "0"
        }
    },
    "response_code": 0,
    "verbose_msg": "OK"
}
 *
 */
