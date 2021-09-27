#include "SecurityTrails.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * Limited to 2000 results for the Free plan and to 10000 for all paid subscriptions.
 */
SecurityTrails::SecurityTrails(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &SecurityTrails::replyFinished);
    ///
    /// get api key....
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("securitytrails").toString();
    Config::generalConfig().endGroup();
}
SecurityTrails::~SecurityTrails(){
    delete manager;
}

void SecurityTrails::start(){
    QNetworkRequest request;
    QUrl url("https://api.securitytrails.com/v1/domain/"+target+"/subdomains?children_only=false&include_inactive=true");
    request.setUrl(url);
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("apikey", m_key.toUtf8());
    manager->get(request);
}

void SecurityTrails::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.object()["subdomains"].toArray();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString().append(".").append(target));
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*
 *
 * Subdomains Return
{
    endpoint: "/v1/domain/trello.com/subdomains",
    subdomains: [
        "admin",
        "api",
        "blog",
        "boards",
        "c",
        "e",
        "em",
        "email",
        "help",
        "info",
        "support",
        "tech",
        "w",
        "ww",
        "www",
        "wwww"
    ]
}

 * DNS History
 *
https://api.securitytrails.com/v1/history/trello.com/dns/a

{
    records: [
        {
            first_seen: "2018-06-08",
            organizations: [
                "Comcast Cable Communications, LLC"
            ],
            last_seen: null,
            values: [
            {
                ip: "104.70.75.9",
                ip_count: 1
            }
            ]
        },
        {...others...}
        ],
    pages: 9,
    endpoint: "/v1/history/trello.com/dns/a",
    type: "a/ipv4"
}

 * WHOIS History
 *
https://api.securitytrails.com/v1/history/twilio.com/whois

{
    result: {
        items: [
        {
            ended: 1512131429698,
            nameServers: [
                "NS0.DNSMADEEASY.COM",
                "NS1.DNSMADEEASY.COM",
                "NS2.DNSMADEEASY.COM",
                "NS3.DNSMADEEASY.COM",
                "NS4.DNSMADEEASY.COM"
            ],
            contact: [
                {
                    name: "Domain Administrator",
                    fax: "1877889455",
                    country: "UNITED STATES",
                    postalCode: "94105",
                    organization: "Twilio Inc.",
                    city: "San Francisco",
                    email: "domains@twilio.com",
                    type: "registrant",
                    telephone: "1877889455",
                    street1: "375 Beale St., Suite 300",
                    state: "CA"
                },
                {...other contacts...}
                ],
            createdDate: 1193405489687,
            expiresDate: 1666791089687,
            updatedDate: 1472736689687,
            registrarName: "MARKMONITOR INC.",
            domain: "twilio.com",
            tld: "com",
            started: 1488372117378,
            status: [
                "clientDeleteProhibited https://icann.org/epp#clientDeleteProhibited",
                "clientTransferProhibited https://icann.org/epp#clientTransferProhibited",
                "clientUpdateProhibited https://icann.org/epp#clientUpdateProhibited"
            ],
            contactEmail: "domains@twilio.com"
        },
        {...others...}
        ],
        count: 19
        },
    endpoint: "/v1/history/twilio.com/whois"
}

 * Associated Domains
 *
https://api.securitytrails.com/v1/domain/twilio.com/associated

{
    records: [
        {
            alexa_rank: 7493,
            whois: {
                createdDate: 1193437578000,
                registrar: "MarkMonitor Inc.",
                expiresDate: 1666823178000
            },
            mail_provider: [
                "Proofpoint, Inc."
            ],
            hostname: "twilio.com",
            host_provider: [
                "Amazon.com, Inc."
            ],
            computed: {
                company_name: "Twilio Inc."
            }
        },
        {...other records...}
    ],
    meta: {
        max_page: 3,
        total_pages: 3,
        query: "whois_email = 'domains@twilio.com'",
        page: 1
    },
    record_count: 282,
    endpoint: "/v1/domain/twilio.com/associated"
}

 * IP Subnet
 *
  --url https://api.securitytrails.com/v1/ips/stats \
  --header 'apikey: >' \
  --header 'content-type: application/json' \
  --data '{
  "query": "ip = '\''5.175.0.0/12'\''"

{
    ports: [
        {
            key: 80,
            count: 35641
        },
        {
            key: 22,
            count: 8796
        },
        {
            key: 443,
            count: 7356
        },
        {...other ports...}
    ],
    top_ptr_patterns: [
        {
            key: "user-x-x-x-x.play-internet.pl",
            count: 65423
        },
        {...other items...}
    ],
    endpoint: "/v1/ips/stats",
    total: 457835
}
*/
