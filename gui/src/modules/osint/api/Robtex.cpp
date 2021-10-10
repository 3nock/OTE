#include "Robtex.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * FIX:
 *      it returns a file instead of json
 * Use as model for ip-scans...
 * returns all record types for particular domain, especially A & AAAA, also NS & MX
 * Mostly for ip-info and domain to ips
 */
Robtex::Robtex(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Robtex::replyFinished);
}
Robtex::~Robtex(){
    delete manager;
}

void Robtex::start(){
    QNetworkRequest request;
    QUrl url("https://freeapi.robtex.com/pdns/forward/"+args->target);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void Robtex::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QString results = QString::fromUtf8(reply->readAll());
        QStringList resultsList = results.simplified().split("\n");
        foreach(const QString &result, resultsList)
        {
            QJsonDocument json = QJsonDocument::fromJson(result.toUtf8());
            QString recordType = json.object()["rrtype"].toString();
            QString ip = json.object()["rrdata"].toString();
            QString name = json.object()["rrname"].toString();
            if(recordType == "A" || recordType == "AAAA")
                emit scanResults(ip);
        }
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*
 * IP-QUERY
https://freeapi.robtex.com/ipquery/199.19.54.1

{
   "status": "ok",
   "city": "Toronto",
   "country": "Canada",
   "as": 12041,
   "asname": "AFILIAS-NST Afilias Limited",
   "whoisdesc": "Afilias Canada, Corp. (ACC-308)",
   "routedesc": "REACH (Customer Route)",
   "bgproute": "199.19.54.0/24"
   "act": [
      {
         "o": "b0.org.afilias-nst.org",
         "t": 1498717654
      }
   ],
   "acth": [],
   "pas": [
      {
         "o": "b0.org.afilias-nst.org",
         "t": 1501358830
      },
      {
         "o": "b0.nic.ngo",
         "t": 1501358850
      },
      {
         "o": "b0.org.afilias-nst.info",
         "t": 1493870770
      },
      {
         "o": "b0.nic.ong",
         "t": 1500758606
      },
      {
         "o": "b0.org.afilias-nst.org",
         "t": 1452591322
      }
   ],
   "pash": [],
}

 * ASN-QUERY
https://freeapi.robtex.com/asquery/1234

{
   "status": "ok",
   "nets": [
      {
         "n": "132.171.0.0/16",
         "inbgp": 1
      },
      {
         "n": "132.171.0.0/17",
         "inbgp": 1
      },
      {
         "n": "132.171.128.0/17",
         "inbgp": 1
      },
      {
         "n": "193.110.32.0/21",
         "inbgp": 1
      },
      {
         "n": "193.110.32.0/22",
         "inbgp": 1
      },
      {
         "n": "193.110.36.0/22",
         "inbgp": 1
      }
   ]
}

 * DNS-QUERY
https://freeapi.robtex.com/pdns/forward/a.iana-servers.net

{"rrname":"a.iana-servers.net","rrdata":"2001:500:8c::53","rrtype":"AAAA","time_first":1441242410,"time_last":1460542918,"count":18}
{"rrname":"a.iana-servers.net","rrdata":"2001:500:8f::53","rrtype":"AAAA","time_first":1460751956,"time_last":1501399246,"count":18}
{"rrname":"a.iana-servers.net","rrdata":"199.43.132.53","rrtype":"A","time_first":1441242410,"time_last":1460542918,"count":18}
{"rrname":"a.iana-servers.net","rrdata":"199.43.135.53","rrtype":"A","time_first":1460751956,"time_last":1501399246,"count":18}

https://freeapi.robtex.com/pdns/reverse/199.43.132.53
{"rrname":"a.iana-servers.org","rrdata":"199.43.132.53","rrtype":"A","time_first":1439620242,"time_last":1460165924,"count":18}
{"rrname":"a.icann-servers.net","rrdata":"199.43.132.53","rrtype":"A","time_first":1448005462,"time_last":1456189254,"count":2}
{"rrname":"a.iana-servers.org","rrdata":"199.43.132.53","rrtype":"A","time_first":1439620242,"time_last":1460165924,"count":18}
{"rrname":"a.iana-servers.net","rrdata":"199.43.132.53","rrtype":"A","time_first":1441242410,"time_last":1460542918,"count":18}

*/
