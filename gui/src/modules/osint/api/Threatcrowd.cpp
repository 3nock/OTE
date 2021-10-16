#include "Threatcrowd.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


Threatcrowd::Threatcrowd(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Threatcrowd::replyFinished);
}
Threatcrowd::~Threatcrowd(){
    delete manager;
}

void Threatcrowd::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "email")
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/email/report/?email="+args->target);
        if(args->option == "domain")
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/domain/report/?domain="+args->target);
        if(args->option == "ip")
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/ip/report/?ip="+args->target);
        if(args->option == "antivirus")
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/antivirus/report/?antivirus="+args->target);
        if(args->option == "file")
            url.setUrl("https://www.threatcrowd.org/searchApi/v2/file/report/?resource="+args->target);
    }else{
        url.setUrl("https://www.threatcrowd.org/searchApi/v2/domain/report/?domain="+args->target);
    }

    request.setUrl(url);
    manager->get(request);
}

void Threatcrowd::replyFinished(QNetworkReply *reply){
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
        if(jsonObject["response_code"].toString() == "1")
        {
            /*
                            FOR IP-ADDRESS
            QJsonArray resolutions = jsonObject["resolutions"].toArray();
            foreach(const QJsonValue &value, resolutions)
                emit subdomain(value["ip_address"].toString());

                              FOR EMAILS
            QJsonArray emails = jsonObject["emails"].toArray();
            foreach(const QJsonValue &value, emails)
                emit subdomain(value.toString());
            */

            QJsonArray subdomains = jsonObject["subdomains"].toArray();
            foreach(const QJsonValue &value, subdomains)
                emit subdomain(value.toString());
        }
    }
    reply->deleteLater();
    emit quitThread();
}
