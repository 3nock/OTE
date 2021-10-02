#include "Suip.h"

Suip::Suip(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Suip::replyFinished);
}
Suip::~Suip(){
    delete manager;
}

void Suip::start(){
    QNetworkRequest request;
    QByteArray data;
    QUrl urlAmass("https://suip.biz/?act=amass");
    QUrl urlSubfinder("https://suip.biz/?act=subfinder");
    QUrl urlFindomain("https://suip.biz/?act=findomain");
    data.append("url="+target);
    data.append("&");
    data.append("Submit1=Submit");
    request.setUrl(urlAmass);
    manager->post(request, data);
}

void Suip::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        emit scanResults(QString::fromUtf8(reply->readAll()));
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

void Suip::getSubdomains(GumboNode *node){
    Q_UNUSED(node);
}
