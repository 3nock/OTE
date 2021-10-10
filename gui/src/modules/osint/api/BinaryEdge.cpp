#include "BinaryEdge.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


/*
 * take note, api access token reduces as more pages are fetched
 */
BinaryEdge::BinaryEdge(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &BinaryEdge::replyFinished);
    ///
    /// getting the api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("binaryedge").toString();
    Config::generalConfig().endGroup();
}
BinaryEdge::~BinaryEdge(){
    delete manager;
}

void BinaryEdge::start(){
    QNetworkRequest request;
    QUrl url("https://api.binaryedge.io/v2/query/domains/subdomain/"+args->target+"?page="+QString::number(m_page));
    request.setUrl(url);
    request.setRawHeader("X-KEY", m_key.toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void BinaryEdge::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        /*
         * QString currentPage = jsonObject["page"].toString();
         * QString pageSize = jsonObject["pagesize"].toString();
         * QString totalSubdomains = jsonObject["total"].toString();
         *
         */
        QJsonArray subdomainList = jsonObject["events"].toArray();
        ///
        /// if the end of the list exit...
        ///
        if(subdomainList.isEmpty())
            goto END;
        ///
        /// if not get all subdomains...
        ///
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
        ///
        /// next query...
        ///
        m_page++;
        QNetworkRequest request;
        QUrl url("https://api.binaryedge.io/v2/query/domains/subdomain/"+args->target+"?page="+QString::number(m_page));
        request.setUrl(url);
        request.setRawHeader("X-KEY", m_key.toUtf8());
        request.setRawHeader("Content-Type", "application/json");
        manager->get(request);
        ///
        /// cleanup and exit function...
        ///
        reply->deleteLater();
        return;
    }
    else{
        emit errorLog(reply->errorString());
    }

END:
    reply->deleteLater();
    emit quitThread();
}

/*
 * OTHER QUERIES...
 *
 * /v2/query/ip/{target}/{cidr}
 *
 * Dataleaks
 * Allows you to search across multiple data breaches to see if any of your
 * email addresses has been compromised.
 * If you are affected, we recommend you change your password on the respective services.
 *
 * /v2/query/dataleaks/email/{email}
 *
 * {
 *  "total": 18,
 *  "events": ["antipublic", "ashleymadison", "lastfm", "linkedin", "mate1", "neopets"],
 *  "query": "user@example.com"
 * }
 *
 * /v2/query/domains/dns/{target}
 * Return list of known DNS results for the target domain.
 *
 * Possible types of records currently available:
 * A
 * AAAA
 * NS
 * MX
{
  "query": "root:example.com",
  "page": 1,
  "pagesize": 100,
  "total": 6308,
  "events": [{
    "A": ["92.63.97.42"],
    "updated_at": "2018-09-22T04:53:21.082802",
    "domain": "startup.antichat.example.com",
    "root": "example.com"
  }, {
    "A": ["93.184.216.34"],
    "MX": ["example.com"],
    "NS": ["ns1.example.com", "ns2.example.com"],
    "updated_at": "2018-12-10T13:20:16.854174",
    "domain": "example.com",
    "root": "example.com",
  }, {
    "A": ["91.235.136.112"],
    "updated_at": "2018-09-22T04:14:29.031596",
    "domain": "vladimirbezz3.example.com",
    "root": "example.com"
  }, {
    "A": ["93.179.68.6"],
    "updated_at": "2018-09-22T03:51:36.852124",
    "domain": "i.seeva.example.com",
    "root": "example.com"
  }]
}
 *
 * /v2/query/domains/ip/{target}
 * Return records that have the specified IP address in their A or AAAA records.
 *
{
  "query": "A:\"8.8.8.8\"",
  "page": 1,
  "pagesize": 100,
  "total": 726,
  "events": [{
    "A": ["8.8.8.8"],
    "updated_at": "2018-06-08T20:51:30.676063",
    "NS": ["ns1058.ui-dns.org", "ns1062.ui-dns.com", "ns1068.ui-dns.biz", "ns1096.ui-dns.de"],
    "domain": "aeroway.co.uk",
    "root": "aeroway.co.uk",
    "MX": ["mx00.1and1.co.uk", "mx01.1and1.co.uk"]
  }, {
    "A": ["8.8.8.8"],
    "updated_at": "2018-06-08T20:53:30.348620",
    "NS": ["f1g1ns1.dnspod.net", "f1g1ns2.dnspod.net"],
    "domain": "84168800.com",
    "root": "84168800.com"
  }, {
    "A": ["8.8.8.8"],
    "updated_at": "2018-06-08T20:53:32.450310",
    "NS": ["f1g1ns1.dnspod.net", "f1g1ns2.dnspod.net"],
    "domain": "84169911.com",
    "root": "84169911.com"
  }, {
    "A": ["8.8.8.8"],
    "updated_at": "2018-06-08T20:53:32.508761",
    "NS": ["f1g1ns1.dnspod.net", "f1g1ns2.dnspod.net"],
    "domain": "84163311.com",
    "root": "84163311.com"
  }, {
    "A": ["8.8.8.8"],
    "updated_at": "2018-06-08T20:53:32.540496",
    "NS": ["f1g1ns1.dnspod.net", "f1g1ns2.dnspod.net"],
    "domain": "00888416.com",
    "root": "00888416.com"
  }]
}

*/
