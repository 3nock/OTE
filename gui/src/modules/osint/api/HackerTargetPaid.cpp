#include "HackerTargetPaid.h"
#include "src/utils/Config.h"

#define ASLOOKUP 0
#define BANNER_GRABBER 1
#define DNSLOOKUP 2
#define FIND_SHARED_DNS 3
#define GEOIP 4
#define HOSTSEARCH 5
#define HTTP_HEADERS 6
#define PAGELINKS 7
#define PING 8
#define REVERSE_DNS 9
#define REVERSE_IPLOOKUP 10
#define SUBNET_CALC 11
#define TRACEROUTE 12
#define WHOIS_LOOKUP 13
#define ZONETRANSFER 14


HackerTargetPaid::HackerTargetPaid(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "HackerTarget";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedRaw);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedAsn);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("hackertarget").toString();
    Config::generalConfig().endGroup();
}
HackerTargetPaid::~HackerTargetPaid(){
    delete manager;
}

void HackerTargetPaid::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case ASLOOKUP:
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+args->target+"&apikey="+m_key);
            break;
        case BANNER_GRABBER:
            url.setUrl("https://api.hackertarget.com/bannerlookup/?q="+args->target+"&apikey="+m_key);
            break;
        case DNSLOOKUP:
            url.setUrl("https://api.hackertarget.com/dnslookup/?q="+args->target+"&apikey="+m_key);
            break;
        case FIND_SHARED_DNS:
            url.setUrl("https://api.hackertarget.com/findshareddns/?q="+args->target+"&apikey="+m_key);
            break;
        case GEOIP:
            url.setUrl("https://api.hackertarget.com/geoip/?q="+args->target+"&apikey="+m_key);
            break;
        case HOSTSEARCH:
            url.setUrl("https://api.hackertarget.com/hostsearch/?q="+args->target+"&apikey="+m_key);
            break;
        case HTTP_HEADERS:
            url.setUrl("https://api.hackertarget.com/httpheaders/?q="+args->target+"&apikey="+m_key);
            break;
        case PAGELINKS:
            url.setUrl("https://api.hackertarget.com/pagelinks/?q="+args->target+"&apikey="+m_key);
            break;
        case PING:
            url.setUrl("https://api.hackertarget.com/nping/?q="+args->target+"&apikey="+m_key);
            break;
        case REVERSE_DNS:
            url.setUrl("https://api.hackertarget.com/reversedns/?q="+args->target+"&apikey="+m_key);
            break;
        case REVERSE_IPLOOKUP:
            url.setUrl("https://api.hackertarget.com/reverseiplookup/?q="+args->target+"&apikey="+m_key);
            break;
        case SUBNET_CALC:
            url.setUrl("https://api.hackertarget.com/subnetcalc/?q="+args->target+"&apikey="+m_key);
            break;
        case TRACEROUTE:
            url.setUrl("https://api.hackertarget.com/mtr/?q="+args->target+"&apikey="+m_key);
            break;
        case WHOIS_LOOKUP:
            url.setUrl("https://api.hackertarget.com/whois/?q="+args->target+"&apikey="+m_key);
            break;
        case ZONETRANSFER:
            url.setUrl("https://api.hackertarget.com/zonetransfer/?q="+args->target+"&apikey="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputDomain){
        if(args->outputSubdomainIp || args->outputSubdomain || args->outputIp){
            url.setUrl("https://api.hackertarget.com/hostsearch/?q="+args->target);
            request.setAttribute(QNetworkRequest::User, HOSTSEARCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputSubdomain || args->outputIp){
            url.setUrl("https://api.hackertarget.com/dnslookup/?q="+args->target);
            request.setAttribute(QNetworkRequest::User, DNSLOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args->inputIp){
        if(args->outputAsn){
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+args->target);
            request.setAttribute(QNetworkRequest::User, ASLOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputSubdomain){
            url.setUrl("https://api.hackertarget.com/reverseiplookup/?q="+args->target);
            request.setAttribute(QNetworkRequest::User, REVERSE_IPLOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void HackerTargetPaid::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();

        if(requestType == HOSTSEARCH){
            QString results = reply->readAll();
            foreach(const QString &item, results.split("\n")){
                QStringList domainAndIp = item.split(","); // ["subdomain", "ip-address"]
                /*
                   some results have only subdomain without an ip-address
                   hence only those with both are accepted
                 */
                if(domainAndIp.size() == 2){
                    emit subdomainIp(domainAndIp.at(0), domainAndIp.at(1));
                    log.resultsCount++;
                }
            }
        }
    }
    end(reply);
}

void HackerTargetPaid::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();

        if(requestType == HOSTSEARCH){
            QString results = reply->readAll();
            foreach(const QString &item, results.split("\n")){
                emit subdomain(item.split(",").at(0));
                log.resultsCount++;
            }
        }

        if(requestType == DNSLOOKUP){
            QString results = reply->readAll();
            foreach(const QString &item, results.split("\n")){
                QString type = item.split(":").at(0);
                QString value = item.split(":").at(1);

                type = type.remove(" ");
                if(type == "NS"){
                    QString ns = value.remove(" ");
                    emit NS(ns);
                    log.resultsCount++;
                }
                if(type == "MX"){
                    QString mx = value.split(" ").at(1);
                    emit MX(mx);
                    log.resultsCount++;
                }
                if(type == "CNAME"){
                    QString cname = value.remove(" ");
                    emit CNAME(cname);
                    log.resultsCount++;
                }
            }
        }

        if(requestType == REVERSE_IPLOOKUP){
            QString results = reply->readAll();
            foreach(const QString &item, results.split("\n")){
                emit subdomain(item);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void HackerTargetPaid::replyFinishedIp(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();

        if(requestType == HOSTSEARCH){
            QString results = reply->readAll();
            foreach(const QString &item, results.split("\n")){
                emit ip(item.split(",").at(1));
                log.resultsCount++;
            }
        }

        if(requestType == DNSLOOKUP){
            QString results = reply->readAll();
            foreach(const QString &item, results.split("\n")){
                QString type = item.split(":").at(0);

                type = type.remove(" ");
                if(type == "A"){
                    QString value = item.split(":").at(0);
                    value = value.remove(" ");
                    emit ipA(value);
                    log.resultsCount++;
                }
                if(type == "AAA"){
                    QString value = item;
                    value = value.remove(0, 7);
                    emit ipAAAA(value);
                    log.resultsCount++;
                }
            }
        }
    }
    end(reply);
}

void HackerTargetPaid::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error())
        this->onError(reply);
    else
    {
        int requestType = reply->property(REQUEST_TYPE).toInt();

        if(requestType == ASLOOKUP){
            QString result = reply->readAll();
            result = result.remove("\"");
            QStringList resultList = result.split(",");

            emit asn(resultList.at(1), resultList.at(3));
            log.resultsCount++;
        }
    }
    end(reply);
}
