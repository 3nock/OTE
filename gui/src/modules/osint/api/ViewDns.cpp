#include "ViewDns.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * only 250 api calls available for free...
 */
ViewDns::ViewDns(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ViewDns::replyFinished);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("viewdns").toString();
    Config::generalConfig().endGroup();
}
ViewDns::~ViewDns(){
    delete manager;
}

void ViewDns::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "Abuse Contact Lookup")
            url.setUrl("https://api.viewdns.info/abuselookup/?domain="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "DNS Propagation Checker")
            url.setUrl("https://api.viewdns.info/propagation/?domain="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "DNS Record Lookup")
            url.setUrl("https://api.viewdns.info/dnsrecord/?domain="+args->target+"&recordtype=ANY&apikey="+m_key+"&output=json");
        if(args->option == "Domain/IP Whois")
            url.setUrl("https://api.viewdns.info/whois/?domain="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "IP History")
            url.setUrl("https://api.viewdns.info/iphistory/?domain="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "IP Location Finder")
            url.setUrl("https://api.viewdns.info/iplocation/?ip="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "MAC Address Lookup")
            url.setUrl("https://api.viewdns.info/maclookup/?mac="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "Ping")
            url.setUrl("https://api.viewdns.info/ping/?host="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "Port Scanner")
            url.setUrl("https://api.viewdns.info/portscan/?host="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "Reverse DNS Lookup")
            url.setUrl("https://api.viewdns.info/reversedns/?ip="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "Reverse IP Lookup")
            url.setUrl("https://api.viewdns.info/reverseip/?host="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "Reverse MX Lookup")
            url.setUrl("https://api.viewdns.info/reversemx/?mx="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "Reverse NS Lookup")
            url.setUrl("https://api.viewdns.info/reversens/?ns="+args->target+"&apikey="+m_key+"&output=json");
        if(args->option == "Reverse Whois Lookup")
            url.setUrl("https://api.viewdns.info/reversewhois/?q="+args->target+"&apikey="+m_key+"&output=json");
    }else{
        /*
         * nothing yet...
         */
    }

    request.setUrl(url);
    manager->get(request);
}

void ViewDns::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        /*
         * Nothing yet...
         */
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
