#include "WhoisXmlApi.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


/*
 * After Sign Up you automaticsubdomainIpy get a free subscription plan limited to 500 queries per month.
 * has a well parsed whois data...
 */
WhoisXmlApi::WhoisXmlApi(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinished);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("whoisxmlapi").toString();
    Config::generalConfig().endGroup();
}
WhoisXmlApi::~WhoisXmlApi(){
    delete manager;
}

void WhoisXmlApi::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "whois")
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&outputFormat=JSON&domainName="+args->target);
        if(args->option == "ipWhois")
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&ipWhois=1&ip=1&domainName="+args->target);
        if(args->option == "DNS Lookup")
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/DNSService?apiKey="+m_key+"&domainName="+args->target+"&type=_subdomainIp");
        if(args->option == "Email Verification")
            url.setUrl("https://emailverification.whoisxmlapi.com/api/v1?apiKey="+m_key+"&emailAddress="+args->target);
    }else{
        url.setUrl("https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
    }

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void WhoisXmlApi::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject result = jsonReply.object()["result"].toObject();
        if(result["count"].toInt()){
            QJsonArray records = result["records"].toArray();
            foreach(const QJsonValue &value, records)
                emit subdomain(value.toObject()["domain"].toString());
        }
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}


/*
 * SUBDOMAINS
 *
https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target
{
    "search":"udsm.ac.tz",
    "result": {
        "count":364,
        "records":[
            {"domain":"mail.google.com","firstSeen":1575656461,"lastSeen":1592219550},
            {"domain":"play.google.com","firstSeen":1546640517,"lastSeen":1592198445}
        ]
    }
}
 *
 *
 * WHOIS
 *
https://www.whoisxmlapi.com/whoisserver/WhoisService?domainName=" + domainName + "&apiKey=" + apiKey + "&outputFormat=JSON"
 *
 *
 * EMAIL-VERIFICATION
 *
"https://emailverification.whoisxmlapi.com/api/v1?url="+api_url+"apiKey="+api_key+"&emailAddress="+email
 *
 * OTHERS
https://reverse-whois.whoisxmlapi.com/api/v2
https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey=" .. key .. "&ip=" .. ip,
https://ip-netblocks.whoisxmlapi.com/api/v2?apiKey=" .. key .. "&asn=" .. tostring(asn)

*/
