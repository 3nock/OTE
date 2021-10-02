#include "Bgpview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * https://api.bgpview.io/asn/as_number
 * https://api.bgpview.io/asn/as_number/prefixes
 * https://api.bgpview.io/asn/as_number/peers
 * https://api.bgpview.io/asn/as_number/upstreams
 * https://api.bgpview.io/asn/as_number/downstreams
 * https://api.bgpview.io/asn/as_number/ixs
 * https://api.bgpview.io/prefix/ip_address/cidr
 * https://api.bgpview.io/ip/ip_address
 * https://api.bgpview.io/ix/ix_id
 * https://api.bgpview.io/search?query_term=query_term
 */

Bgpview::Bgpview(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Bgpview::replyFinished);
}
Bgpview::~Bgpview(){
    delete manager;
}

void Bgpview::start(){
    QNetworkRequest request;
    QUrl url("https://api.bgpview.io/search?query_term="+target);
    request.setUrl(url);
    manager->get(request);
}

void Bgpview::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray subdomainList = jsonReply.array();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}
