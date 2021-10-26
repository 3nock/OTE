#include "Bgpview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ASN 0
#define ASN_DOWNSTREAM 1
#define ASN_IXS 2
#define ASN_PEERS 3
#define ASN_PREFIXES 4
#define ASN_UPSTREAMS 5
#define IP 6
#define IP_PREFIXES 7
#define QUERY 8

Bgpview::Bgpview(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Bgpview";

    if(args->raw)
        connect(manager, &MyNetworkAccessManager::finished, this, &Bgpview::replyFinishedRaw);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Bgpview::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Bgpview::replyFinishedIp);
    if(args->outputAsn)
        connect(manager, &MyNetworkAccessManager::finished, this, &Bgpview::replyFinishedAsn);
    if(args->outputEmail)
        connect(manager, &MyNetworkAccessManager::finished, this, &Bgpview::replyFinishedEmail);
}
Bgpview::~Bgpview(){
    delete manager;
}

void Bgpview::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    if(args->raw){
        switch(args->rawOption){
        case ASN:
            url.setUrl("https://api.bgpview.io/asn/"+args->target);
            break;
        case ASN_DOWNSTREAM:
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/downstreams");
            break;
        case ASN_IXS:
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/ixs");
            break;
        case ASN_PEERS:
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/peers");
            break;
        case ASN_PREFIXES:
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/prefixes");
            break;
        case ASN_UPSTREAMS:
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/upstreams");
            break;
        case IP:
            url.setUrl("https://api.bgpview.io/ip/"+args->target);
            break;
        case IP_PREFIXES:
            url.setUrl("https://api.bgpview.io/prefix/"+args->target);
            break;
        case QUERY:
            url.setUrl("https://api.bgpview.io/search?query_term="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        url.setUrl("https://api.bgpview.io/search?query_term="+args->target);
        request.setAttribute(QNetworkRequest::User, QUERY);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputIp){
        url.setUrl("https://api.bgpview.io/ip/"+args->target);
        request.setAttribute(QNetworkRequest::User, IP);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputAsn){
        if(args->outputSubdomain){
            url.setUrl("https://api.bgpview.io/asn/"+args->target);
            request.setAttribute(QNetworkRequest::User, ASN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args->outputEmail){
            url.setUrl("https://api.bgpview.io/asn/"+args->target);
            request.setAttribute(QNetworkRequest::User, ASN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args->outputIp){
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/prefixes");
            request.setAttribute(QNetworkRequest::User, ASN_PREFIXES);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args->outputAsn){
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/peers");
            request.setAttribute(QNetworkRequest::User, ASN_PEERS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/downstreams");
            request.setAttribute(QNetworkRequest::User, ASN_DOWNSTREAM);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/upstreams");
            request.setAttribute(QNetworkRequest::User, ASN_UPSTREAMS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Bgpview::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == IP){
        QJsonArray prefixes = data["prefixes"].toArray();
        foreach(const QJsonValue &value, prefixes){
            QString ipValue = value.toObject()["ip"].toString();
            emit ip(ipValue);
            log.resultsCount++;
        }
        QJsonArray related_prefixes = data["related_prefixes"].toArray();
        foreach(const QJsonValue &value, related_prefixes){
            QString ipValue = value.toObject()["ip"].toString();
            emit ip(ipValue);
            log.resultsCount++;
        }
    }

    /* asn prefixes and query return results in similar format */
    if(requestType == ASN_PREFIXES || requestType == QUERY){
        QJsonArray ipv4_prefixes = data["ipv4_prefixes"].toArray();
        foreach(const QJsonValue &value, ipv4_prefixes){
            QString ipValue = value.toObject()["ip"].toString();
            emit ipA(ipValue);
            log.resultsCount++;
        }
        QJsonArray ipv6_prefixes = data["ipv6_prefixes"].toArray();
        foreach(const QJsonValue &value, ipv6_prefixes){
            QString ipValue = value.toObject()["ip"].toString();
            emit ipAAAA(ipValue);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Bgpview::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    /* asn peers, asn upstreams and asn downstreams return results in similar format */
    if(requestType == ASN_PEERS || requestType == ASN_DOWNSTREAM || requestType == ASN_UPSTREAMS){
        QStringList keys = data.keys();
        foreach(const QString &key, keys){
            QJsonArray asnList = data[key].toArray();
            foreach(const QJsonValue &value, asnList){
                QString asnValue = QString::number(value.toObject()["asn"].toInt());
                QString asnName = value.toObject()["name"].toString();
                //...
                emit asn(asnValue, asnName);
                log.resultsCount++;
            }
        }
    }

    /* ip and query return results in similar format */
    if(requestType == IP || requestType == QUERY){
        QJsonArray asnList = data["asns"].toArray();
        foreach(const QJsonValue &value, asnList){
            QString asnValue = QString::number(value.toObject()["asn"].toInt());
            QString asnName = value.toObject()["name"].toString();
            //...
            emit asn(asnValue, asnName);
            log.resultsCount++;
        }
    }
    end(reply);
}

void Bgpview::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == QUERY){
        QStringList keys = data.keys();
        foreach(const QString &key, keys){
            foreach(const QJsonValue &value, data[key].toArray()){
                QJsonArray emailList = value.toObject()["email_contacts"].toArray();
                foreach(const QJsonValue &value, emailList){
                    emit email(value.toString());
                    log.resultsCount++;
                }
            }
        }
    }

    /* ip and asn results contains email contacts in same format */
    if(requestType == IP || requestType == ASN){
        QJsonArray emails = data["email_contacts"].toArray();
        foreach(const QJsonValue &value, emails){
            emit email(value.toString());
            log.resultsCount++;
        }
    }
    end(reply);
}

void Bgpview::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    int requestType = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(requestType == ASN){
        QString domain = data["website"].toString();
        domain = domain.remove(0, 8).remove("/");
        emit subdomain(domain);
        log.resultsCount++;
    }
    end(reply);
}
