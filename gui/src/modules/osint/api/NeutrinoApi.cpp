#include "NeutrinoApi.h"
#include "src/utils/Config.h"

#define EMAIL_VALIDATE 0
#define HOST_REPUTATION 1
#define IP_BLOCKLIST 2
#define IP_INFO 3
#define IP_PROBE 4
#define PHONE_VALIDATE 5
#define UA_LOOKUP 6
#define URL_INFO 7

/*
 * 50 free request per day...
 */
NeutrinoApi::NeutrinoApi(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "NeutrinoApi";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &NeutrinoApi::replyFinishedRawJson);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("neutrinoapi_key").toString();
    m_userId = Config::generalConfig().value("neutrinoapi_uid").toString();
    Config::generalConfig().endGroup();
}
NeutrinoApi::~NeutrinoApi(){
    delete manager;
}

void NeutrinoApi::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case EMAIL_VALIDATE:
            url.setUrl("https://neutrinoapi.net/email-validate?email="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        case HOST_REPUTATION:
            url.setUrl("https://neutrinoapi.net/host-reputation?host="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        case IP_BLOCKLIST:
            url.setUrl("https://neutrinoapi.net/ip-blocklist?ip="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        case IP_INFO:
            url.setUrl("https://neutrinoapi.net/ip-info?reverse-lookup=true&ip="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        case IP_PROBE:
            url.setUrl("https://neutrinoapi.net/ip-probe?ip="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        case PHONE_VALIDATE:
            url.setUrl("https://neutrinoapi.net/phone-validate?number="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        case UA_LOOKUP:
            url.setUrl("https://neutrinoapi.net/ua-lookup?ua="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        case URL_INFO:
            url.setUrl("https://neutrinoapi.net/url-info?url="+args->target+"&user-id="+m_userId+"&api-key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

