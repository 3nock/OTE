#include "C99.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


C99::C99(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &C99::replyFinished);
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
        switch (args->rawOption) {
        case 0: // alexa rank checker
            url.setUrl("https://api.c99.nl/alexarank?key="+m_key+"&url="+args->target+"&json");
            break;
        case 1: // disposable mail check
            url.setUrl("https://api.c99.nl/disposablemailchecker?key="+m_key+"&email="+args->target+"&json");
            break;
        case 2: // domain checker
            url.setUrl("https://api.c99.nl/domainchecker?key="+m_key+"&domain="+args->target+"&json");
            break;
        case 3: // domain history checker
            url.setUrl("https://api.c99.nl/domainhistory?key="+m_key+"&domain="+args->target+"&json");
            break;
        case 4: // email validator
            url.setUrl("https://api.c99.nl/emailvalidator?key="+m_key+"&email="+args->target+"&json");
            break;
        case 5: // WAF checker
            url.setUrl("https://api.c99.nl/firewsubdomainIpdetector?key="+m_key+"&url="+args->target+"&json");
            break;
        case 6: // GeoIp
            url.setUrl("https://api.c99.nl/geoip?key="+m_key+"&host="+args->target+"&json");
            break;
        case 7: // Host to Ip
            url.setUrl("https://api.c99.nl/dnsresolver?key="+m_key+"&host="+args->target+"&json");
            break;
        case 8: // Ip 2 domains
            url.setUrl("https://api.c99.nl/ip2domains?key="+m_key+"&ip="+args->target+"&json");
            break;
        case 9: // Ip Logger
            url.setUrl("https://api.c99.nl/iplogger?key="+m_key+"&action=viewloggers&json");
            break;
        case 10: // IP validator
            url.setUrl("https://api.c99.nl/ipvalidator?key="+m_key+"&ip="+args->target+"&json");
            break;
        case 11: // Ip 2 Host
            url.setUrl("https://api.c99.nl/gethostname?key="+m_key+"&host="+args->target+"&json");
            break;
        case 12: // IP 2 Skype
            url.setUrl("https://api.c99.nl/ip2skype?key="+m_key+"&ip="+args->target+"&json");
            break;
        case 13: // link backup
            url.setUrl("https://api.c99.nl/linkbackup?key="+m_key+"&url="+args->target+"&json");
            break;
        case 14: // multiple port scanner
            url.setUrl("https://api.c99.nl/portscanner?key="+m_key+"&host="+args->target+"&json");
            break;
        case 15: // nmap scanner
            url.setUrl("https://api.c99.nl/nmap?key="+m_key+"&host="+args->target+"&json");
            break;
        case 16: // phone lookup
            url.setUrl("https://api.c99.nl/phonelookup?key="+m_key+"&number="+args->target+"&json");
            break;
        case 17: // ping
            url.setUrl("https://api.c99.nl/ping?key="+m_key+"&host="+args->target+"&json");
            break;
        case 18: // proxy detector
            url.setUrl("https://api.c99.nl/proxydetector?key="+m_key+"&ip="+args->target+"&json");
            break;
        case 19: // screenshot tool
            url.setUrl("https://api.c99.nl/createscreenshot?key="+m_key+"&url="+args->target+"&json");
            break;
        case 20: // Site/URL Reputation Checker
            url.setUrl("https://api.c99.nl/reputationchecker?key="+m_key+"&url="+args->target+"&json");
            break;
        case 21: // Skype Resolver
            url.setUrl("https://api.c99.nl/skyperesolver?key="+m_key+"&username="+args->target+"&json");
            break;
        case 22: // Subdomain Finder
            url.setUrl("https://api.c99.nl/subdomainfinder?key="+m_key+"&domain="+args->target+"&json");
            break;
        case 23: // tor checker
            url.setUrl("https://api.c99.nl/torchecker?key="+m_key+"&ip="+args->target+"&json");
            break;
        case 24: // Website Headers
            url.setUrl("https://api.c99.nl/getheaders?key="+m_key+"&host="+args->target+"&json");
            break;
        case 25: // Website Up or Down Checker
            url.setUrl("https://api.c99.nl/upordown?key="+m_key+"&host="+args->target+"&json");
            break;
        case 26: // Whois Checker
            url.setUrl("https://api.c99.nl/whois?key="+m_key+"&domain="+args->target+"&json");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
    /*
     * Others not yet implemented...
     */
}

void C99::replyFinished(QNetworkReply *reply){
    log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error())
        this->onError(reply);
    else
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            goto END;
        }

        /* jsonObject */
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();

        /* For subdomains results */
        if(args->outputSubdomain){
            bool success = jsonObject["success"].toBool();
            if(success){
                QJsonArray subdomainList = jsonObject["subdomains"].toArray();
                foreach(const QJsonValue &value, subdomainList)
                    emit subdomain(value.toString());
            }
        }

        /* Others not yet Implemented */
    }

END:
    reply->deleteLater();
    activeRequests--;
    if(activeRequests == 0){
        //emit infoLog(log);
        emit quitThread();
    }
}
