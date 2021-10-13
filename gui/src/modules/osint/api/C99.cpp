#include "C99.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


C99::C99(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &C99::replyFinished);
    ///
    /// get api key....
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("c99").toString();
    Config::generalConfig().endGroup();
}
C99::~C99(){
    delete manager;
}

void C99::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "Subdomain Finder")
            url.setUrl("https://api.c99.nl/subdomainfinder?key="+m_key+"&domain="+args->target+"&json");
        if(args->option == "Phone Lookup")
            url.setUrl("https://api.c99.nl/phonelookup?key="+m_key+"&number="+args->target+"&json");
        if(args->option == "Skype Resolver")
            url.setUrl("https://api.c99.nl/skyperesolver?key="+m_key+"&username="+args->target+"&json");
        if(args->option == "Ip 2 Skype")
            url.setUrl("https://api.c99.nl/ip2skype?key="+m_key+"&ip="+args->target+"&json");
        if(args->option == "Firewall Technology Detector")
            url.setUrl("https://api.c99.nl/firewalldetector?key="+m_key+"&url="+args->target+"&json");
        if(args->option == "Multiple port scanner")
            url.setUrl("https://api.c99.nl/portscanner?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "Nmap Scanner")
            url.setUrl("https://api.c99.nl/nmap?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "Ping")
            url.setUrl("https://api.c99.nl/ping?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "IP to Host")
            url.setUrl("https://api.c99.nl/gethostname?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "Host to IP")
            url.setUrl("https://api.c99.nl/dnsresolver?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "Domain Checker")
            url.setUrl("https://api.c99.nl/domainchecker?key="+m_key+"&domain="+args->target+"&json");
        if(args->option == "Domain History Check")
            url.setUrl("https://api.c99.nl/domainhistory?key="+m_key+"&domain="+args->target+"&json");
        if(args->option == "IP 2 Domains")
            url.setUrl("https://api.c99.nl/ip2domains?key="+m_key+"&ip="+args->target+"&json");
        if(args->option == "Alexa Rank Checker")
            url.setUrl("https://api.c99.nl/alexarank?key="+m_key+"&url="+args->target+"&json");
        if(args->option == "Whois Checker")
            url.setUrl("https://api.c99.nl/whois?key="+m_key+"&domain="+args->target+"&json");
        if(args->option == "Screenshot Tool")
            url.setUrl("https://api.c99.nl/createscreenshot?key="+m_key+"&url="+args->target+"&json");
        if(args->option == "GeoIP")
            url.setUrl("https://api.c99.nl/geoip?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "Website Up or Down Checker")
            url.setUrl("https://api.c99.nl/upordown?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "Site/URL Reputation Checker")
            url.setUrl("https://api.c99.nl/reputationchecker?key="+m_key+"&url="+args->target+"&json");
        if(args->option == "Website Headers")
            url.setUrl("https://api.c99.nl/getheaders?key="+m_key+"&host="+args->target+"&json");
        if(args->option == "Link Backup")
            url.setUrl("https://api.c99.nl/linkbackup?key="+m_key+"&url="+args->target+"&json");
        if(args->option == "Email Validator")
            url.setUrl("https://api.c99.nl/emailvalidator?key="+m_key+"&email="+args->target+"&json");
        if(args->option == "Disposable Mail Check")
            url.setUrl("https://api.c99.nl/disposablemailchecker?key="+m_key+"&email="+args->target+"&json");
        if(args->option == "IP Validator")
            url.setUrl("https://api.c99.nl/ipvalidator?key="+m_key+"&ip="+args->target+"&json");
        if(args->option == "TOR Checker")
            url.setUrl("https://api.c99.nl/torchecker?key="+m_key+"&ip="+args->target+"&json");
        if(args->option == "IP Logger")
            url.setUrl("https://api.c99.nl/iplogger?key="+m_key+"&action=viewloggers&json");
        if(args->option == "Proxy Detector")
            url.setUrl("https://api.c99.nl/proxydetector?key="+m_key+"&ip="+args->target+"&json");
    }else{
        url.setUrl("https://api.c99.nl/subdomainfinder?key="+m_key+"&domain="+args->target+"&json");
    }

    request.setUrl(url);
    manager->get(request);
}

void C99::replyFinished(QNetworkReply *reply){
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
        ///
        /// check if the request was successful...
        ///
        bool success = jsonObject["success"].toBool();
        if(success){
            QJsonArray subdomainList = jsonObject["subdomains"].toArray();
            foreach(const QJsonValue &value, subdomainList)
                emit scanResults(value.toString());
        }
        else{
            QString error = jsonObject["error"].toString();
            emit scanResults(error);
        }
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
