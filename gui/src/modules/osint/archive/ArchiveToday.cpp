#include "ArchiveToday.h"


ArchiveToday::ArchiveToday(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Archive";

    if(args->outputUrl)
        connect(manager, &MyNetworkAccessManager::finished, this, &ArchiveToday::replyFinishedUrl);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &ArchiveToday::replyFinishedSubdomain);
}
ArchiveToday::~ArchiveToday(){
    delete manager;
}

void ArchiveToday::start(){
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    QUrl url;
    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("http://archive.is/*."+args->target);
            request.setUrl(url);
            manager->get(request);
        }
        if(args->outputUrl){
            url.setUrl("http://archive.is/*."+args->target+"/*");
            request.setUrl(url);
            manager->get(request);
        }
    }
}

void ArchiveToday::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    GumboOutput *output = gumbo_parse(reply->readAll());
    this->m_getSubdomains(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

void ArchiveToday::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    GumboOutput *output = gumbo_parse(reply->readAll());
    this->m_getUrls(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

void ArchiveToday::m_getSubdomains(GumboNode *node){
    Q_UNUSED(node);
}

void ArchiveToday::m_getUrls(GumboNode *node){
    Q_UNUSED(node);
}
