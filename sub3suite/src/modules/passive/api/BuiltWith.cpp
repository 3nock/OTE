#include "BuiltWith.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define COMPANY_API 0
#define DOMAIN_API 1
#define FREE_API 2
#define KEYWORD_API 3
#define LIST_API 4
#define LIVE_API 5
#define REDIRECTS_API 6
#define RELATIONSHIP_API 7
#define TRENDS_API 8
#define TRUST_API 9


/*
 * for now only raw results...
 * for free api, 1 request per second...
 */
BuiltWith::BuiltWith(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_BUILTWITH;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &BuiltWith::replyFinishedRawJson);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_BUILTWITH).toString();
}
BuiltWith::~BuiltWith(){
    delete manager;
}

void BuiltWith::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case FREE_API:
            url.setUrl("https://api.builtwith.com/free1/api.json?KEY="+m_key+"&LOOKUP="+target);
            break;
        case DOMAIN_API:
            url.setUrl("https://api.builtwith.com/v19/api.json?KEY="+m_key+"&LOOKUP="+target);
            break;
        case LIST_API:
            url.setUrl("https://api.builtwith.com/lists8/api.json?KEY="+m_key+"&TECH="+target);
            break;
        case RELATIONSHIP_API:
            url.setUrl("https://api.builtwith.com/rv1/api.json?KEY="+m_key+"&LOOKUP="+target);
            break;
        case REDIRECTS_API:
            url.setUrl("https://api.builtwith.com/redirect1/api.json?KEY="+m_key+"&LOOKUP="+target);
            break;
        case KEYWORD_API:
            url.setUrl("https://api.builtwith.com/kw2/api.json?KEY="+m_key+"&LOOKUP="+target);
            break;
        case TRENDS_API:
            url.setUrl("https://api.builtwith.com/trends/v6/api.json?KEY="+m_key+"&TECH="+target);
            break;
        case COMPANY_API:
            url.setUrl("https://ctu.builtwith.com/ctu1/api.json?KEY="+m_key+"&COMPANY="+target);
            break;
        case LIVE_API:
            url.setUrl("https://api.builtwith.com/dlv2/api.json?KEY="+m_key+"&LOOKUP="+target);
            break;
        case TRUST_API:
            url.setUrl("https://api.builtwith.com/trustv1/api.json?KEY="+m_key+"&LOOKUP="+target);
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
