#include "CommonCrawl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * for now only use the first index url to obtain crawled urls...
 */
CommonCrawl::CommonCrawl(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "CommonCrawl";
}
CommonCrawl::~CommonCrawl(){
    delete manager;
}

void CommonCrawl::start(){
    /* first temporary connection to index */
    connect(manager, &s3sNetworkAccessManager::finished, this, &CommonCrawl::replyFinishedIndex);

    /* request to obtain the index url */
    QNetworkRequest request;
    QUrl url("https://index.commoncrawl.org/collinfo.json");
    request.setUrl(url);
    manager->get(request);
    activeRequests++;
}

void CommonCrawl::replyFinishedIndex(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
    QJsonArray subdomainList = jsonReply.array();

    foreach(const QJsonValue &value, subdomainList)
    {
        QJsonObject aaa = value.toObject();
        urlList.append(aaa["cdx-api"].toString());
    }

    /* disconnect the first manager connection */
    disconnect(manager, &s3sNetworkAccessManager::finished, this, &CommonCrawl::replyFinishedIndex);

    /* make new manager connection depending on user output */
    if(args.outputUrl)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CommonCrawl::replyFinishedUrl);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CommonCrawl::replyFinishedSubdomain);

    /* send request to get the subdomains/urls */
    QUrl url(urlList.at(0)+"?url=*."+target+"&output=json&fl=url");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
    activeRequests++;

    end(reply);
}

void CommonCrawl::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString replyList = QString::fromUtf8(reply->readAll());
    QStringList urlList = replyList.split("\n");

    foreach(const QString &value, urlList)
    {
        QString urlValue = value;
        urlValue.chop(2);
        emit resultUrl(urlValue.remove(0, 9));
        log.resultsCount++;
    }

    end(reply);
}

void CommonCrawl::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QString replyList = QString::fromUtf8(reply->readAll());
    QStringList urlList = replyList.split("\n");

    foreach(const QString &value, urlList)
    {
        /* obtaining the url */
        QString urlValue = value;
        urlValue.chop(2);
        QString domainUrl = urlValue.remove(0, 9);

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
