#include "ArchiveIt.h"

ArchiveIt::ArchiveIt(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ArchiveIt::replyFinished);
}
ArchiveIt::~ArchiveIt(){
    delete manager;
}

void ArchiveIt::start(){
    QNetworkRequest request;
    QUrl url("https://wayback.archive-it.org/all/timemap/cdx?matchType=domain&fl=original&collapse=urlkey&url="+target);
    request.setUrl(url);
    manager->get(request);
}

void ArchiveIt::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QString document = QString::fromUtf8(reply->readAll());
        QStringList urlList = document.remove(" ").split("\n");
        foreach(const QString &url, urlList)
            emit scanResults(url);
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
