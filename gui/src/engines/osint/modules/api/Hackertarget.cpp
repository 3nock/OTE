#include "Hackertarget.h"

/*
 *  also asn lookup...
 *      https://api.hackertarget.com/aslookup/?q=1.1.1.1
 *
 */
Hackertarget::Hackertarget(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Hackertarget::replyFinished);
}
Hackertarget::~Hackertarget(){
    delete manager;
}

void Hackertarget::start(){
    QNetworkRequest request;
    QUrl url("https://api.hackertarget.com/hostsearch/?q="+target);
    request.setUrl(url);
    manager->get(request);
}

void Hackertarget::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        emit errorLog(reply->errorString());
    }
    else
    {
        /*
         * contains subdomain,ip-address
         */
        QString results = reply->readAll();
        QStringList subdomainList = results.split("\n");
        foreach(const QString &subdomain, subdomainList)
            emit scanResults(subdomain);
    }
    reply->deleteLater();
    emit quitThread();
}
