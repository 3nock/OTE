#include "Mnemonic.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * access passive-dns, 1000 queries per day for public usage
 * you can query for any dns record type
 */

Mnemonic::Mnemonic(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Mnemonic::replyFinished);
}
Mnemonic::~Mnemonic(){
    delete manager;
}

void Mnemonic::start(){
    QNetworkRequest request;
    /* any of dns record can be queried...
    QUrl url("https://api.mnemonic.no/pdns/v3/"+target+"?limit=999&rrType=A&rrType=AAAA&rrType=PTR&rrType=CNAME&rrType=MX");
    */
    QUrl url("https://api.mnemonic.no/pdns/v3/"+target+"?limit=999");
    request.setUrl(url);
    manager->get(request);
}

void Mnemonic::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject results = jsonReply.object();
        ///
        /// ....
        ///
        int responseCode = results["responseCode"].toInt();
        if(responseCode == 200){
            //int count = results["count"].toInt();
            QJsonArray data = results["data"].toArray();
            foreach(const QJsonValue &value, data){
                QString recordType = value.toObject()["rrtype"].toString();
                QString answer = value.toObject()["answer"].toString();
                ///
                /// sending results...
                ///
                if(recordType == "a" || recordType == "aaaa")
                    emit scanResults(answer);
                if(recordType == "mx")
                    emit scanResults(answer);
                if(recordType == "cname")
                    emit scanResults(answer);
                if(recordType == "ptr")
                    emit scanResults(answer);
            }
        }
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*
 * RESULTS FORMAT
{
    "responseCode": 200, # the response code. Normal responses should return HTTP code 200
    "count": 20,         # the total number of matching records. If this value is lower than the imposed limit, the resultset is truncated!
    "limit": 25,         # the limit imposed on the query results (default is 25). Use the limit parameter in the request to set a higher limit.
    "offset": 0,         # the offset applied on the query results (default i 0)
    "currentPage": 1,    # the current "page" (calculated from limit/offset)
    "size": 20,          # the size of the current resultset (should be same as count or limit)
    "data": [],          # the list of query result objects
    "messages": [],      # any server messages
    "metaData": {},      # any server metadata
}

{
    "rrclass": "in",                      # the DNS record class
    "rrtype": "a",                        # the DNS record type
    "query": "cnn.com.",                  # the DNS record query part
    "answer": "157.166.255.19",           # the DNS record answer part
    "firstSeenTimestamp": 1340308340000,  # the first registered timestamp for this record
    "lastSeenTimestamp": 1377520248000,   # the last registered timestamp for this record
    "maxTtl": 300,                        # the maximum TTL observed for this record
    "minTtl": 300,                        # the minimum TTL observed for this record
    "times": 675,                         # the number of times this record has been observed
    "tlp": "white",                       # the TLP of this record. Public records have TLP "white"
    "customer": null,                     # the customer owning this record. If null, this is an aggregated or anonymous record.
    "createdTimestamp": 0,                # not in use
    "lastUpdatedTimestamp": 0,            # not in use
}

*/
