#include "HybridAnalysis.h"
#include "src/utils/Config.h"

#define URL_QUICKSCAN 0


HybridAnalysis::HybridAnalysis(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "HybridAnalysis";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &HybridAnalysis::replyFinishedRaw);
    ///
    /// get api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("hybridanalysis").toString();
    Config::generalConfig().endGroup();
}
HybridAnalysis::~HybridAnalysis(){
    delete manager;
}

void HybridAnalysis::start(){
    QNetworkRequest request;
    request.setRawHeader("api-key", m_key.toUtf8());
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("user-agent", "Falcon Sandbox");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QUrl url;
    if(args->raw){
        switch (args->rawOption) {
        case URL_QUICKSCAN:
            url.setUrl("https://www.hybrid-analysis.com/api/v2/quick-scan/url");
            QByteArray data;
            data.append("scan_type=all&");
            data.append("url="+args->target);
            request.setUrl(url);
            manager->post(request, data);
            activeRequests++;
            break;
        }
    }
}
