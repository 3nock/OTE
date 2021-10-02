#include "IpInfo.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


/*
 * 50,000 requests per month
 */

IpInfo::IpInfo(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &IpInfo::replyFinished);
    ///
    /// getting the api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("ipinfo").toString();
    Config::generalConfig().endGroup();
}
IpInfo::~IpInfo(){
    delete manager;
}

void IpInfo::start(){
    QNetworkRequest request;
    QUrl url("https://ipinfo.io/"+target+"/json?token="+m_key);
    request.setRawHeader("Accept", "application/json");
    request.setUrl(url);
    manager->get(request);
}

void IpInfo::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject results = jsonReply.object();
        ///
        /// getting results...
        ///
        QString hostname = results["hostname"].toString();
        QString city = results["city"].toString();
        QString region = results["region"].toString();
        QString country = results["country"].toString();
        QString location = results["loc"].toString();
        QString organization = results["org"].toString();
        ///
        /// send results...
        ///
        emit scanResults(hostname);
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*
 * FREE PLAN
{
  "ip": "66.87.125.72",
  "hostname": "ip-66-87-125-72.spfdma.spcsdns.net",
  "city": "Springfield",
  "region": "Massachusetts",
  "country": "US",
  "loc": "42.1015,-72.5898",
  "org": "AS10507 Sprint Personal Communications Systems",
  "postal": "01101",
  "timezone": "America/New_York"
}

 * BASIC PLAN
{
  "ip": "66.87.125.72",
  "hostname": "ip-66-87-125-72.spfdma.spcsdns.net",
  "city": "Springfield",
  "region": "Massachusetts",
  "country": "US",
  "loc": "42.1015,-72.5898",
  "org": "AS10507 Sprint Personal Communications Systems",
  "postal": "01101",
  "timezone": "America/New_York",
  "asn": {
    "asn": "AS10507",
    "name": "Sprint Personal Communications Systems",
    "domain": "sprint.net",
    "route": "66.87.125.0/24",
    "type": "isp"
  }
}

 * BUSSINESS PLAN
{
  "ip": "8.8.8.8",
  "hostname": "dns.google",
  "anycast": true,
  "city": "Mountain View",
  "region": "California",
  "country": "US",
  "loc": "37.4056,-122.0775",
  "postal": "94043",
  "timezone": "America/Los_Angeles",
  "asn": {
    "asn": "AS15169",
    "name": "Google LLC",
    "domain": "google.com",
    "route": "8.8.8.0/24",
    "type": "business"
  },
  "company": {
    "name": "Google LLC",
    "domain": "google.com",
    "type": "business"
  },
  "privacy": {
    "vpn": false,
    "proxy": false,
    "tor": false,
    "hosting": false
  },
  "abuse": {
    "address": "US, CA, Mountain View, 1600 Amphitheatre Parkway, 94043",
    "country": "US",
    "email": "network-abuse@google.com",
    "name": "Abuse",
    "network": "8.8.8.0/24",
    "phone": "+1-650-253-0000"
  },
  "domains": {
    "ip": "8.8.8.8",
    "total": 11606,
    "domains": [
        "41.cn",
        "onionflix.cc",
        "newmax.info",
        "ftempurl.com",
        "itempurl.com"
    ]
  }
}

ipinfo.io/domains/8.8.8.8?token=650e5ad807db50
{
  "ip": "8.8.8.8",
  "total": 11606,
  "domains": [
    "41.cn",
    "onionflix.cc",
    "newmax.info",
    "ftempurl.com",
    "itempurl.com"
  ]
}
 * PREMIUM-PLAN
{
  "ip": "8.8.8.8",
  "hostname": "dns.google",
  "anycast": true,
  "city": "Mountain View",
  "region": "California",
  "country": "US",
  "loc": "37.4056,-122.0775",
  "postal": "94043",
  "timezone": "America/Los_Angeles",
  "asn": {
    "asn": "AS15169",
    "name": "Google LLC",
    "domain": "google.com",
    "route": "8.8.8.0/24",
    "type": "business"
  },
  "company": {
    "name": "Google LLC",
    "domain": "google.com",
    "type": "business"
  },
  "privacy": {
    "vpn": false,
    "proxy": false,
    "tor": false,
    "hosting": false
  },
  "abuse": {
    "address": "US, CA, Mountain View, 1600 Amphitheatre Parkway, 94043",
    "country": "US",
    "email": "network-abuse@google.com",
    "name": "Abuse",
    "network": "8.8.8.0/24",
    "phone": "+1-650-253-0000"
  },
  "domains": {
    "ip": "8.8.8.8",
    "total": 11606,
    "domains": [
        "41.cn",
        "onionflix.cc",
        "newmax.info",
        "ftempurl.com",
        "itempurl.com"
    ]
  }
}

IP Ranges API

ipinfo.io/ranges/comcast.net?token=650e5ad807db50
{
  "domain": "comcast.net",
  "num_ranges": "37330",
  "ranges": [
    "23.24.240.0/29",
    "23.24.240.64/29",
    "23.24.240.128/28",
    "23.24.240.152/29",
    "23.24.240.168/29",
    "23.24.240.192/29",
    "23.24.240.208/29",
    "23.24.241.40/29",
    "23.24.241.72/29",
    "23.24.241.96/29",
    "23.24.241.112/28",
    "23.24.241.136/29",
    "23.24.241.168/29",
    "23.24.241.184/29",
    ...
   ]
}

Hosted Domains API
ipinfo.io/domains/8.8.8.8?token=650e5ad807db50
{
  "ip": "8.8.8.8",
  "total": "37330",
  "domains": [
    "41.cn",
    "onionflix.cc",
    "newmax.info",
    "ftempurl.com",
    "itempurl.com",
    "authrock.com",
    "ctempurl.com",
    "mtqnia.com",
    "server-panel.net",
    "gtempurl.com",
    "htempurl.com",
    ...
   ]
}

*/
