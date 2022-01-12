#include "Whoxy.h"
#include "src/utils/Config.h"

#define ACCOUNT_BALANCE_CHECK 0
#define REVERSE_WHOIS_COMPANY 1
#define REVERSE_WHOIS_EMAIL 2
#define REVERSE_WHOIS_KEYWORD 3
#define REVERSE_WHOIS_NAME 4
#define WHOIS_HISTORY 5
#define WHOIS_LOOKUP 6

Whoxy::Whoxy(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "Whoxy";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Whoxy::replyFinishedRawJson);
    ///
    /// getting api key...
    ///
    
    m_key = APIKEY.value("whoxy").toString();
    
}
Whoxy::~Whoxy(){
    delete manager;
}

void Whoxy::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ACCOUNT_BALANCE_CHECK:
            url.setUrl("http://api.whoxy.com/?key="+m_key+"&account=balance");
            break;
        case WHOIS_LOOKUP:
            url.setUrl("http://api.whoxy.com/?key="+m_key+"&whois="+target+"&format=json");
            break;
        case WHOIS_HISTORY:
            url.setUrl("http://api.whoxy.com/?key="+m_key+"&history="+target+"&format=json");
            break;
        case REVERSE_WHOIS_NAME:
            url.setUrl("http://api.whoxy.com/?key="+m_key+"&reverse=whois&name="+target+"&format=json");
            break;
        case REVERSE_WHOIS_EMAIL:
            url.setUrl("http://api.whoxy.com/?key="+m_key+"&reverse=whois&email="+target+"&format=json");
            break;
        case REVERSE_WHOIS_COMPANY:
            url.setUrl("http://api.whoxy.com/?key="+m_key+"&reverse=whois&company="+target+"&format=json");
            break;
        case REVERSE_WHOIS_KEYWORD:
            url.setUrl("http://api.whoxy.com/?key="+m_key+"&reverse=whois&keyword="+target+"&format=json");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
