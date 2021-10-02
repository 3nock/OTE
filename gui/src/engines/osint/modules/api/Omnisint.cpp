#include "Omnisint.h"
#include <QJsonDocument>
#include <QJsonArray>

/*
    https://sonar.omnisint.io
    /subdomains/{domain} - All subdomains for a given domain
    /tlds/{domain} - All tlds found for a given domain
    /all/{domain} - All results across all tlds for a given domain
    /reverse/{ip} - Reverse DNS lookup on IP address
    /reverse/{ip}/{mask} - Reverse DNS lookup of a CIDR range


           ip-address to Subdomain names
    1.1.1.1/24 ==> ip/mask

   {
      "1.1.1.97": [
        "static-96-238-191-97.rcmdva.fios.verizon.net"
        "one.one.one.one"
      ],
      "1.1.1.98": [
        "static-96-238-191-98.rcmdva.fios.verizon.net"
      ],
      "1.1.1.99": [
        "wism-virtual.loopback.nyumc.org"
      ]
   }
*/

Omnisint::Omnisint(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Omnisint::replyFinished);
}
Omnisint::~Omnisint(){
    delete manager;
}

void Omnisint::start(){
    QNetworkRequest request;
    m_page = 1;
    QUrl url("https://sonar.omnisint.io/all/"+target+"?page="+QString::number(m_page));
    request.setUrl(url);
    manager->get(request);
}

void Omnisint::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
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
            QUrl url("https://sonar.omnisint.io/all/"+target+"?page="+QString::number(m_page));
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

