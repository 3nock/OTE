#include "Hackertarget.h"

/*
 *  also asn lookup...
 *      https://api.hackertarget.com/aslookup/?q=1.1.1.1
 *
 */

/*
 * contains subdomain,ip-address
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

    QUrl url;
    if(args->raw){
        if(args->option == "subdomains&Ip")
            url.setUrl("https://api.hackertarget.com/hostsearch/?q="+args->target);
        if(args->option == "asnLookup")
            url.setUrl("https://api.hackertarget.com/aslookup/?q="+args->target);
    }else{
        url.setUrl("https://api.hackertarget.com/hostsearch/?q="+args->target);
    }

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

        QString results = reply->readAll();
        QStringList subdomainList = results.split("\n");
        foreach(const QString &item, subdomainList)
            emit subdomain(item);
    }
    reply->deleteLater();
    emit quitThread();
}
