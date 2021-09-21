#include "ArchiveToday.h"


ArchiveToday::ArchiveToday(QString _target, QObject *parent):
    AbstractModule (parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ArchiveToday::replyFinished);
}
ArchiveToday::~ArchiveToday(){
    delete manager;
}

void ArchiveToday::start(){
    QNetworkRequest request;
    QUrl url("http://archive.is/*."+target);
    request.setUrl(url);
    manager->get(request);
}

void ArchiveToday::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){

    }else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

void ArchiveToday::getSubdomains(GumboNode *node){
    Q_UNUSED(node);
}

void ArchiveToday::getScreenshots(GumboNode *node){
    Q_UNUSED(node);
}

