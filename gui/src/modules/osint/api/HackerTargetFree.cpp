#include "HackerTargetFree.h"

#define ASLOOKUP 0
#define DNSLOOKUP 1
#define REVERSEDNS 2
#define REVERSEIPLOOKIP 3
#define PAGELINKS 4
#define HOSTSEARCH 5


HackerTargetFree::HackerTargetFree(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &HackerTargetFree::replyFinished);
    //...
    log.moduleName = "HackerTarget";
    log.resultsCount = 0;
}
HackerTargetFree::~HackerTargetFree(){
    delete manager;
}

void HackerTargetFree::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case 0: // asLookup
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+args->target);
            break;
        case 1: // dnsLookup
            url.setUrl("https://api.hackertarget.com/dnslookup/?q="+args->target);
            break;
        case 2: // geoip
            url.setUrl("https://api.hackertarget.com/geoip/?q="+args->target);
            break;
        case 3: // httpheaders
            url.setUrl("https://api.hackertarget.com/httpheaders/?q="+args->target);
            break;
        case 4: // pageLinks
            url.setUrl("https://api.hackertarget.com/pagelinks/?q="+args->target);
            break;
        case 5: // reversedns
            url.setUrl("https://api.hackertarget.com/reversedns/?q="+args->target);
            break;
        case 6: // reverseIpLookup
            url.setUrl("https://api.hackertarget.com/reverseiplookup/?q="+args->target);
            break;
        }
        request.setUrl(url);
        request.setAttribute(QNetworkRequest::User, HOSTSEARCH);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputDomain){
        if(args->outputSubdomainIp || args->outputSubdomain || args->outputIp){
            url.setUrl("https://api.hackertarget.com/hostsearch/?q="+args->target);
            request.setUrl(url);
            request.setAttribute(QNetworkRequest::User, HOSTSEARCH);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputSubdomain || args->outputIp){
            /*
            url.setUrl("https://api.hackertarget.com/dnslookup/?q="+args->target);
            request.setUrl(url);
            manager->get(request);
            manager->setProperty(REQUEST_TYPE, DNSLOOKUP);
            activeRequests++;
            */
        }
    }

    if(args->inputIp){
        if(args->outputAsn){
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+args->target);
            request.setUrl(url);
            manager->get(request);
            manager->setProperty(REQUEST_TYPE, ASLOOKUP);
            activeRequests++;
        }
        if(args->outputSubdomainIp || args->outputSubdomain){
            url.setUrl("https://api.hackertarget.com/reversedns/?q="+args->target);
            request.setUrl(url);
            manager->get(request);
            manager->setProperty(REQUEST_TYPE, REVERSEDNS);
            activeRequests++;
        }
        if(args->outputSubdomain){
            url.setUrl("https://api.hackertarget.com/reverseiplookup/?q="+args->target);
            request.setUrl(url);
            manager->get(request);
            manager->setProperty(REQUEST_TYPE, REVERSEIPLOOKIP);
            activeRequests++;
        }
    }
}

void HackerTargetFree::replyFinished(QNetworkReply *reply){
    log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error())
        this->onError(reply);
    else
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            goto END;
        }

        int requestType = reply->property(REQUEST_TYPE).toInt();

        if(args->outputAsn){
            /*
                Not yet implemented...
             */
            goto END;
        }

        /* for the remaining */
        QString results = reply->readAll();
        QStringList subdomainList = results.split("\n"); // ["subdomain,ip-address", ...]

        if(args->outputSubdomainIp && requestType == HOSTSEARCH){
            foreach(const QString &item, subdomainList){
                QStringList domainAndIp = item.split(","); // ["subdomain", "ip-address"]
                emit subdomainIp(domainAndIp.at(0), domainAndIp.at(1));
                log.resultsCount++;
            }
            goto END;
        }

        if(args->outputSubdomain && requestType == HOSTSEARCH){
            foreach(const QString &item, subdomainList){
                emit subdomain(item.split(",")[0]); // subdomain
                log.resultsCount++;
            }
            goto END;
        }

        if(args->outputIp && requestType == HOSTSEARCH){
            foreach(const QString &item, subdomainList){
                emit ip(item.split(",")[1]); // ip-address
                log.resultsCount++;
            }
        }
    }

END:
    reply->deleteLater();
    activeRequests--;
    if(activeRequests == 0){
        //emit infoLog(log);
        emit quitThread();
    }
}
