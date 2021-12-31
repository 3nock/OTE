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


HackerTargetPaid::HackerTargetPaid(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "HackerTarget";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedRawTxt);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedSubdomainIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetPaid::replyFinishedAsn);
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
    if(args.outputRaw){
        switch(args.rawOption){
        case ASLOOKUP:
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+target+"&apikey="+m_key);
            break;
        case BANNER_GRABBER:
            url.setUrl("https://api.hackertarget.com/bannerlookup/?q="+target+"&apikey="+m_key);
            break;
        case DNSLOOKUP:
            url.setUrl("https://api.hackertarget.com/dnslookup/?q="+target+"&apikey="+m_key);
            break;
        case FIND_SHARED_DNS:
            url.setUrl("https://api.hackertarget.com/findshareddns/?q="+target+"&apikey="+m_key);
            break;
        case GEOIP:
            url.setUrl("https://api.hackertarget.com/geoip/?q="+target+"&apikey="+m_key);
            break;
        case HOSTSEARCH:
            url.setUrl("https://api.hackertarget.com/hostsearch/?q="+target+"&apikey="+m_key);
            break;
        case HTTP_HEADERS:
            url.setUrl("https://api.hackertarget.com/httpheaders/?q="+target+"&apikey="+m_key);
            break;
        case PAGELINKS:
            url.setUrl("https://api.hackertarget.com/pagelinks/?q="+target+"&apikey="+m_key);
            break;
        case PING:
            url.setUrl("https://api.hackertarget.com/nping/?q="+target+"&apikey="+m_key);
            break;
        case REVERSE_DNS:
            url.setUrl("https://api.hackertarget.com/reversedns/?q="+target+"&apikey="+m_key);
            break;
        case REVERSE_IPLOOKUP:
            url.setUrl("https://api.hackertarget.com/reverseiplookup/?q="+target+"&apikey="+m_key);
            break;
        case SUBNET_CALC:
            url.setUrl("https://api.hackertarget.com/subnetcalc/?q="+target+"&apikey="+m_key);
            break;
        case TRACEROUTE:
            url.setUrl("https://api.hackertarget.com/mtr/?q="+target+"&apikey="+m_key);
            break;
        case WHOIS_LOOKUP:
            url.setUrl("https://api.hackertarget.com/whois/?q="+target+"&apikey="+m_key);
            break;
        case ZONETRANSFER:
            url.setUrl("https://api.hackertarget.com/zonetransfer/?q="+target+"&apikey="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args.inputDomain){
        if(args.outputSubdomainIp || args.outputSubdomain || args.outputIp){
            url.setUrl("https://api.hackertarget.com/hostsearch/?q="+target);
            request.setAttribute(QNetworkRequest::User, HOSTSEARCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.outputSubdomain || args.outputIp){
            url.setUrl("https://api.hackertarget.com/dnslookup/?q="+target);
            request.setAttribute(QNetworkRequest::User, DNSLOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputIp){
        if(args.outputAsn){
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+target);
            request.setAttribute(QNetworkRequest::User, ASLOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.outputSubdomain){
            url.setUrl("https://api.hackertarget.com/reverseiplookup/?q="+target);
            request.setAttribute(QNetworkRequest::User, REVERSE_IPLOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void HackerTargetPaid::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == HOSTSEARCH){
        QString results = reply->readAll();
        foreach(const QString &item, results.split("\n")){
            QStringList domainAndIp = item.split(","); // ["subdomain", "ip-address"]
            /*
               some results have only subdomain without an ip-address
               hence only those with both are accepted
             */
            if(domainAndIp.size() == 2){
                emit resultSubdomainIp(domainAndIp.at(0), domainAndIp.at(1));
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void HackerTargetPaid::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == HOSTSEARCH){
        QString results = reply->readAll();
        foreach(const QString &item, results.split("\n")){
            emit resultSubdomain(item.split(",").at(0));
            log.resultsCount++;
        }
    }

    if(QUERY_TYPE == DNSLOOKUP){
        QString results = reply->readAll();
        foreach(const QString &item, results.split("\n")){
            QString type = item.split(":").at(0);
            QString value = item.split(":").at(1);

            type = type.remove(" ");
            if(type == "NS"){
                QString ns = value.remove(" ");
                emit resultNS(ns);
                log.resultsCount++;
            }
            if(type == "MX"){
                QString mx = value.split(" ").at(1);
                emit resultMX(mx);
                log.resultsCount++;
            }
            if(type == "CNAME"){
                QString cname = value.remove(" ");
                emit resultCNAME(cname);
                log.resultsCount++;
            }
        }
    }

    if(QUERY_TYPE == REVERSE_IPLOOKUP){
        QString results = reply->readAll();
        foreach(const QString &item, results.split("\n")){
            emit resultSubdomain(item);
            log.resultsCount++;
        }
    }

    end(reply);
}

void HackerTargetPaid::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == HOSTSEARCH){
        QString results = reply->readAll();
        foreach(const QString &item, results.split("\n")){
            emit resultIp(item.split(",").at(1));
            log.resultsCount++;
        }
    }

    if(QUERY_TYPE == DNSLOOKUP){
        QString results = reply->readAll();
        foreach(const QString &item, results.split("\n")){
            QString type = item.split(":").at(0);

            type = type.remove(" ");
            if(type == "A"){
                QString value = item.split(":").at(0);
                value = value.remove(" ");
                emit resultA(value);
                log.resultsCount++;
            }
            if(type == "AAA"){
                QString value = item;
                value = value.remove(0, 7);
                emit resultAAAA(value);
                log.resultsCount++;
            }
        }
    }
    end(reply);
}

void HackerTargetPaid::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();

    if(QUERY_TYPE == ASLOOKUP){
        QString result = reply->readAll();
        result = result.remove("\"");
        QStringList resultList = result.split(",");

        emit resultASN(resultList.at(1), resultList.at(3));
        log.resultsCount++;
    }
    end(reply);
}
