#include "WhoisXmlApi.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define DNS_LOOKUP 0
#define EMAIL_VERIFICATION 1
#define IP_WHOIS 2
#define WHOIS 3

/*
 * After Sign Up you automatically get a free subscription plan limited to 500 queries per month.
 * has a well parsed whois data...
 */
WhoisXmlApi::WhoisXmlApi(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "WhoisXmlApi";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &WhoisXmlApi::replyFinishedSubdomain);
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
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args->raw){
        switch (args->rawOption){
        case WHOIS:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&outputFormat=JSON&domainName="+args->target);
            break;
        case IP_WHOIS:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/WhoisService?apiKey="+m_key+"&ipWhois=1&ip=1&domainName="+args->target);
            break;
        case DNS_LOOKUP:
            url.setUrl("https://www.whoisxmlapi.com/whoisserver/DNSService?apiKey="+m_key+"&domainName="+args->target+"&type=_subdomainIp");
            break;
        case EMAIL_VERIFICATION:
            url.setUrl("https://emailverification.whoisxmlapi.com/api/v1?apiKey="+m_key+"&emailAddress="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void WhoisXmlApi::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject result = document.object()["result"].toObject();

    if(result["count"].toInt()){
        QJsonArray records = result["records"].toArray();
        foreach(const QJsonValue &value, records){
            emit subdomain(value.toObject()["domain"].toString());
            log.resultsCount++;
        }
    }

    end(reply);
}


/*
 * SUBDOMAINS
 *
https://subdomains.whoisxmlapi.com/api/v1?apiKey="+m_key+"&domainName="+target
{
    "search":"google",
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
