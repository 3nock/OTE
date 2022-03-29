#include "ArchiveIt.h"

#define URL 0


ArchiveIt::ArchiveIt(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_ARCHIVEIT;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ArchiveIt::replyFinishedRawNdjson);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ArchiveIt::replyFinishedUrl);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ArchiveIt::replyFinishedSubdomain);
}
ArchiveIt::~ArchiveIt(){
    delete manager;
}

void ArchiveIt::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch (args.raw_query_id) {
        case URL:
            url.setUrl("https://wayback.archive-it.org/all/timemap/cdx?matchType=domain&fl=original&collapse=urlkey&url="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_URL || args.output_Hostname){
            url.setUrl("https://wayback.archive-it.org/all/timemap/cdx?matchType=domain&fl=original&collapse=urlkey&url="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_URL){
        if(args.output_URL || args.output_Hostname){
            url.setUrl("https://wayback.archive-it.org/all/timemap/cdx?matchType=domain&fl=original&collapse=urlkey&url="+target);
            request.setUrl(url);
            manager->get(request);
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
        emit resultURL(urlValue);
        log.resultsCount++;
    }

    this->end(reply);
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

    this->end(reply);
}
