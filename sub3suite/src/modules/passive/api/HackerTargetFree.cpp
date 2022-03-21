#include "HackerTargetFree.h"

#define ASLOOKUP 0
#define BANNER_GRABBER 1
#define DNSLOOKUP 2
#define FIND_SHARED_DNS 3
#define GEOIP 4
#define HOSTSEARCH 5
#define HTTP_HEADERS 6
#define PAGELINKS 7
#define REVERSE_DNS 8
#define REVERSE_IPLOOKUP 9
#define SUBNET_CALC 10
#define ZONETRANSFER 11


HackerTargetFree::HackerTargetFree(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_HACKERTARGET;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetFree::replyFinishedRawTxt);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetFree::replyFinishedSubdomainIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetFree::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetFree::replyFinishedIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetFree::replyFinishedAsn);
    if(args.outputCidr)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HackerTargetFree::replyFinishedCidr);
}
HackerTargetFree::~HackerTargetFree(){
    delete manager;
}

void HackerTargetFree::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch(args.rawOption){
        case ASLOOKUP:
            if(!target.startsWith("AS", Qt::CaseInsensitive))
                target = "AS"+target;
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+target);
            break;
        case BANNER_GRABBER:
            url.setUrl("https://api.hackertarget.com/bannerlookup/?q="+target);
            break;
        case DNSLOOKUP:
            url.setUrl("https://api.hackertarget.com/dnslookup/?q="+target);
            break;
        case FIND_SHARED_DNS:
            url.setUrl("https://api.hackertarget.com/findshareddns/?q="+target);
            break;
        case GEOIP:
            url.setUrl("https://api.hackertarget.com/geoip/?q="+target);
            break;
        case HOSTSEARCH:
            url.setUrl("https://api.hackertarget.com/hostsearch/?q="+target);
            break;
        case HTTP_HEADERS:
            url.setUrl("https://api.hackertarget.com/httpheaders/?q="+target);
            break;
        case PAGELINKS:
            url.setUrl("https://api.hackertarget.com/pagelinks/?q="+target);
            break;
        case REVERSE_DNS:
            url.setUrl("https://api.hackertarget.com/reversedns/?q="+target);
            break;
        case REVERSE_IPLOOKUP:
            url.setUrl("https://api.hackertarget.com/reverseiplookup/?q="+target);
            break;
        case SUBNET_CALC:
            url.setUrl("https://api.hackertarget.com/subnetcalc/?q="+target);
            break;
        case ZONETRANSFER:
            url.setUrl("https://api.hackertarget.com/zonetransfer/?q="+target);
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

    if(args.inputAsn){
        if(args.outputCidr){
            if(!target.startsWith("AS", Qt::CaseInsensitive))
                target = "AS"+target;
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+target);
            request.setAttribute(QNetworkRequest::User, ASLOOKUP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void HackerTargetFree::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString results = reply->readAll();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOSTSEARCH:
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

void HackerTargetFree::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString results = reply->readAll();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOSTSEARCH:
        foreach(const QString &item, results.split("\n")){
            emit resultSubdomain(item.split(",").at(0));
            log.resultsCount++;
        }
        break;

    case REVERSE_IPLOOKUP:
        foreach(const QString &item, results.split("\n")){
            emit resultSubdomain(item);
            log.resultsCount++;
        }
    }

    end(reply);
}

void HackerTargetFree::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString results = reply->readAll();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case HOSTSEARCH:
        foreach(const QString &item, results.split("\n")){
            emit resultIP(item.split(",").at(1));
            log.resultsCount++;
        }
    }

    end(reply);
}

void HackerTargetFree::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString result = reply->readAll();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASLOOKUP:
        result = result.remove("\"");
        QStringList resultList = result.split(",");

        emit resultASN(resultList.at(1), resultList.at(3));
        log.resultsCount++;
    }

    end(reply);
}

void HackerTargetFree::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString results = reply->readAll();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASLOOKUP:
        QStringList cidrs = results.split("\n");
        cidrs.removeAt(0);
        foreach(const QString &cidr, cidrs){
            emit resultCIDR(cidr);
            log.resultsCount++;
        }
    }

    end(reply);
}
