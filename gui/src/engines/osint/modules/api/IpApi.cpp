#include "IpApi.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


/*
 * Provides info about an ip-address
 * simple ip-info lookup
 * 1,000 per month for free-user...
 */
IpApi::IpApi(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &IpApi::replyFinished);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipapi").toString();
    Config::generalConfig().endGroup();
}
IpApi::~IpApi(){
    delete manager;
}

void IpApi::start(){
    QNetworkRequest request;
    QUrl url("http://api.ipapi.com/api/"+target+"?access_key="+m_key);
    request.setUrl(url);
    manager->get(request);
}

void IpApi::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        /*
            Nothing for now...
        */
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*
 * SIMPLE IP-LOOKUP
 *
{
    "ip": "1.1.1.1",
    "type": "ipv4",
    "continent_code": "OC",
    "continent_name": "Oceania",
    "country_code": "AU",
    "country_name": "Australia",
    "region_code": "NSW",
    "region_name": "New South Wales",
    "city": "Sydney",
    "zip": "2000",
    "latitude": -33.86714172363281,
    "longitude": 151.2071075439453,
    "location": {
        "geoname_id": 2147714,
        "capital": "Canberra",
        "languages": [{"code": "en", "name": "English", "native": "English"}],
        "country_flag": "https://assets.ipstack.com/flags/au.svg",
        "country_flag_emoji": "\ud83c\udde6\ud83c\uddfa",
        "country_flag_emoji_unicode": "U+1F1E6 U+1F1FA",
        "calling_code": "61", "is_eu": false
    }
}
 OR
{
    "ip": "161.185.160.93",
    "hostname": "161.185.160.93",
    "type": "ipv4",
    "continent_code": "NA",
    "continent_name": "North America",
    "country_code": "US",
    "country_name": "United States",
    "region_code": "NY",
    "region_name": "New York",
    "city": "Brooklyn",
    "zip": "11238",
    "latitude": 40.676,
    "longitude": -73.9629,
    "location": {
        "geoname_id": 5110302,
        "capital": "Washington D.C.",
        "languages": [
            {
                "code": "en",
                "name": "English",
                "native": "English"
            }
        ],
        "country_flag": "http://assets.ipapi.com/flags/us.svg",
        "country_flag_emoji": "🇺🇸",
        "country_flag_emoji_unicode": "U+1F1FA U+1F1F8",
        "calling_code": "1",
        "is_eu": false
    },
    "time_zone": {
        "id": "America/New_York",
        "current_time": "2018-09-24T05:07:10-04:00",
        "gmt_offset": -14400,
        "code": "EDT",
        "is_daylight_saving": true
    },
    "currency": {
        "code": "USD",
        "name": "US Dollar",
        "plural": "US dollars",
        "symbol": "$",
        "symbol_native": "$"
    },
    "connection": {
        "asn": 22252,
        "isp": "The City of New York"
    },
    "security": {
        "is_proxy": false,
        "proxy_type": null,
        "is_crawler": false,
        "crawler_name": null,
        "crawler_type": null,
        "is_tor": false,
        "threat_level": "low",
        "threat_types": null
    }
}

 * Bulk IP Lookup
 *
http://api.ipapi.com/api/161.185.160.93,87.111.168.248,210.138.184.5?access_key=YOUR_ACCESS_KEY

{
    "ip": "161.185.160.93",
    "type": "ipv4",
    "continent_code": "NA",
    "continent_name": "North America",
    "country_code": "US",
    "country_name": "United States",
    "region_code": "NY",
    "region_name": "New York",
    "city": "Brooklyn",
    "zip": "11238",
    "latitude": 40.676,
    "longitude": -73.9629,
    [...]
},
{
    "ip": "161.185.160.93",
    "type": "ipv4",
    "continent_code": "EU",
    "continent_name": "Europe",
    "country_code": "ES",
    "country_name": "Spain",
    "region_code": "GA",
    "region_name": "Galicia",
    "city": "Santiago de Compostela",
    "zip": "15781",
    "latitude": 42.88,
    "longitude": -8.5448,
    [...]
}
*/
