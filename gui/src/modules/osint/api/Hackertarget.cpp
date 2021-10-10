#include "Hackertarget.h"

/*
 *  also asn lookup...
 *      https://api.hackertarget.com/aslookup/?q=1.1.1.1
 *
 */
Hackertarget::Hackertarget(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Hackertarget::replyFinished);
}
Hackertarget::~Hackertarget(){
    delete manager;
}

void Hackertarget::start(){
    QNetworkRequest request;
    QUrl url("https://api.hackertarget.com/hostsearch/?q="+args->target);
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
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
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
