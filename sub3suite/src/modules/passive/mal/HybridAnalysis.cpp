#include "HybridAnalysis.h"
#include "src/utils/Config.h"

#define URL_QUICKSCAN 0


HybridAnalysis::HybridAnalysis(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_HYBRIDANALYSIS;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &HybridAnalysis::replyFinishedRawJson);

    /* get api key */
    m_key = APIKEY.value(OSINT_MODULE_HYBRIDANALYSIS).toString();
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

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case URL_QUICKSCAN:
            url.setUrl("https://www.hybrid-analysis.com/api/v2/quick-scan/url");
            QByteArray data;
            data.append("scan_type=all&");
            data.append("url="+target);
            request.setUrl(url);
            manager->post(request, data);
            break;
        }
    }
}
