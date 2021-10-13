#include "Omnisint.h"
#include <QJsonDocument>
#include <QJsonArray>


Omnisint::Omnisint(ScanArgs *args):
    AbstractOsintModule(args)
{
    ;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Omnisint::replyFinished);
}
Omnisint::~Omnisint(){
    delete manager;
}

void Omnisint::start(){
    QNetworkRequest request;

    QUrl url;
    if(args->raw){
        if(args->option == "subdomains")
            url.setUrl("https://sonar.omnisint.io/subdomains/"+args->target);
        if(args->option == "tlds")
            url.setUrl("https://sonar.omnisint.io/tlds/"+args->target);
        if(args->option == "all")
            url.setUrl("https://sonar.omnisint.io/all/"+args->target);
        if(args->option == "reverse")
            url.setUrl("https://sonar.omnisint.io/reverse/"+args->target);
    }else{
        m_page = 1;
        url.setUrl("https://sonar.omnisint.io/all/"+args->target+"?page="+QString::number(m_page));
    }

    request.setUrl(url);
    manager->get(request);
}

void Omnisint::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomains = jsonReply.array();
        if(!subdomains.isEmpty()){
            ///
            /// obtain all subdomains...
            ///
            foreach(const QJsonValue &value, subdomains)
                emit scanResults(value.toString());
            ///
            /// send another request for other comming pages...
            ///
            m_page++;
            QNetworkRequest request;
            QUrl url("https://sonar.omnisint.io/all/"+args->target+"?page="+QString::number(m_page));
            request.setUrl(url);
            manager->get(request);
            ///
            /// ...
            ///
            reply->deleteLater();
        }
        else{
            reply->deleteLater();
            emit quitThread();
        }
    }
    else
    {
        emit errorLog(reply->errorString());
        reply->deleteLater();
        emit quitThread();
    }
}

