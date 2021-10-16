#include "Threatminer.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Threatminer::Threatminer(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Threatminer::replyFinished);
}
Threatminer::~Threatminer(){
    delete manager;
}

void Threatminer::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "domain whois")
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=1");
        if(args->option == "domain passive dns")
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=2");
        if(args->option == "domain subdomains")
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=5");
        if(args->option == "domain Query URI")
            url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=3");
        if(args->option == "ip whois")
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=1");
        if(args->option == "ip passive dns")
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=2");
        if(args->option == "ip Query URI")
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=3");
        if(args->option == "SSL Certificates")
            url.setUrl("https://api.threatminer.org/v2/host.php?q="+args->target+"&rt=5");
        if(args->option == "SSL Hosts")
            url.setUrl("	https://api.threatminer.org/v2/ssl.php?q="+args->target+"&rt=1");
    }else{
        url.setUrl("https://api.threatminer.org/v2/domain.php?q="+args->target+"&rt=5");
    }

    request.setUrl(url);
    manager->get(request);
}

void Threatminer::replyFinished(QNetworkReply *reply){
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
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        if(jsonObject["status_code"].toString() == "200")
        {
            QJsonArray results = jsonObject["results"].toArray();
            foreach(const QJsonValue &value, results)
                emit subdomain(value.toString());
        }
        else
        {
            emit errorLog("An Error Ocurred: "+jsonObject["status_code"].toString());
        }
    }
    reply->deleteLater();
    emit quitThread();
}
