#include "ArchiveIt.h"

#define URL 0


ArchiveIt::ArchiveIt(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "ArchiveIt";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &ArchiveIt::replyFinishedRawNdjson);
    if(args.outputUrl)
        connect(manager, &NetworkAccessManager::finished, this, &ArchiveIt::replyFinishedUrl);
    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &ArchiveIt::replyFinishedSubdomain);
}
ArchiveIt::~ArchiveIt(){
    delete manager;
}

void ArchiveIt::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case URL:
            url.setUrl("https://wayback.archive-it.org/all/timemap/cdx?matchType=domain&fl=original&collapse=urlkey&url="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputUrl || args.outputSubdomain){
            url.setUrl("https://wayback.archive-it.org/all/timemap/cdx?matchType=domain&fl=original&collapse=urlkey&url="+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void ArchiveIt::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString document = QString::fromUtf8(reply->readAll());
    QStringList urlList = document.remove(" ").split("\n");

    foreach(const QString &urlValue, urlList){
        emit resultUrl(urlValue);
        log.resultsCount++;
    }

    end(reply);
}

void ArchiveIt::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString document = QString::fromUtf8(reply->readAll());
    QStringList urlList = document.remove(" ").split("\n");

    foreach(const QString &url, urlList){
        /* getting url */
        QString domainUrl = url;

        /* extracting subdomain from url...*/
        domainUrl.remove("http://");
        domainUrl.remove("https://");
        domainUrl = domainUrl.split("/").at(0);

        /*  emiting subdomain... */
        emit resultSubdomain(domainUrl);
        log.resultsCount++;
    }

    end(reply);
}
