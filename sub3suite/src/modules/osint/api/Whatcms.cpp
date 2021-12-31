#include "Whatcms.h"
#include "src/utils/Config.h"

#define CMS_DETECTION 0
#define HOST_DETECTION 1
#define STATUS_ENDPOINT 2
#define TECHNOLOGY_DETECTION 3
#define TECHNOLOGY_LIST 4
#define WP_THEME_DETECTION 5


Whatcms::Whatcms(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "Whatcms";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Whatcms::replyFinishedRawJson);

    /* getting api key... */
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("whatcms").toString();
    Config::generalConfig().endGroup();
}
Whatcms::~Whatcms(){
    delete manager;
}

void Whatcms::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case TECHNOLOGY_DETECTION:
            url.setUrl("https://whatcms.org/API/Tech?key="+m_key+"&url="+target);
            break;
        case TECHNOLOGY_LIST:
            url.setUrl("https://whatcms.org/API/List");
            break;
        case CMS_DETECTION:
            url.setUrl("https://whatcms.org/API/CMS?key="+m_key+"&url="+target);
            break;
        case WP_THEME_DETECTION:
            url.setUrl("https://www.themedetect.com/API/Theme?key="+m_key+"&url="+target);
            break;
        case HOST_DETECTION:
            url.setUrl("https://www.who-hosts-this.com/API/Host?key="+m_key+"&url="+target);
            break;
        case STATUS_ENDPOINT:
            url.setUrl("https://whatcms.org/API/Status?key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}
