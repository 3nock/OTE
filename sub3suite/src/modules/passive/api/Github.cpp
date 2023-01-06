#include "Github.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define CODE 0

/*
 * for now only raw results
 */
Github::Github(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_GITHUB;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Github::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Github::replyFinishedSubdomain);

    /* getting the api-key */
    m_key = gConfig.keys.Github;
}
Github::~Github(){
    delete manager;
}

void Github::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("Authorization", "token "+m_key.toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case CODE:
            url.setUrl("https://api.github.com/search/code?q="+target+"&type=Code&page=1&per_page=100");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        url.setUrl("https://api.github.com/search/code?q="+target+"&type=Code&page=1&per_page=100");
        request.setUrl(url);
        request.setAttribute(QNetworkRequest::User, CODE);
        manager->get(request);
        return;
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
    this->end(reply);
}
