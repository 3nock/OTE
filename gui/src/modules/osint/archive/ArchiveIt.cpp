#include "ArchiveIt.h"

ArchiveIt::ArchiveIt(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &ArchiveIt::replyFinished);
}
ArchiveIt::~ArchiveIt(){
    delete manager;
}

void ArchiveIt::start(){
    QNetworkRequest request;
    QUrl url("https://wayback.archive-it.org/subdomainIp/timemap/cdx?matchType=domain&fl=original&collapse=urlkey&url="+args->target);
    request.setUrl(url);
    manager->get(request);
}

void ArchiveIt::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QString document = QString::fromUtf8(reply->readAll());
        QStringList urlList = document.remove(" ").split("\n");
        foreach(const QString &url, urlList)
            emit subdomain(url);
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
