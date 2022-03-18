#include "Github.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define CODE 0

Github::Github(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_GITHUB;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Github::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Github::replyFinishedSubdomain);

    /* getting the api-key */
    m_key = APIKEY.value(OSINT_MODULE_GITHUB).toString();
    
}
Github::~Github(){
    delete manager;
}

void Github::start(){
    QNetworkRequest request;
    request.setRawHeader("Authorization", "token "+m_key.toUtf8());
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args.outputRaw){
        switch(args.rawOption){
        case CODE:
            url.setUrl("https://api.github.com/search/code?q="+target+"&type=Code&page=1&per_page=100");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        url.setUrl("https://api.github.com/search/code?q="+target+"&type=Code&page=1&per_page=100");
        request.setUrl(url);
        request.setAttribute(QNetworkRequest::User, CODE);
        manager->get(request);
        activeRequests++;
    }

}

void Github::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }
    /*
     * Not Yet Implemented...
     */
    end(reply);
}
