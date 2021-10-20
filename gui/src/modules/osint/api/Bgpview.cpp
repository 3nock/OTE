#include "Bgpview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


Bgpview::Bgpview(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &Bgpview::replyFinished);
    //...
    log.moduleName = "Bgpview";
    log.resultsCount = 0;
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
        case 0: // asn
            url.setUrl("https://api.bgpview.io/asn/"+args->target);
            break;
        case 1: // asn downstreams
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/downstreams");
            break;
        case 2: // asn ixs
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/ixs");
            break;
        case 3: // asn peers
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/peers");
            break;
        case 4: // asn prefixes
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/prefixes");
            break;
        case 5: // asn upstreams
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/upstreams");
            break;
        case 6: // ip
            url.setUrl("https://api.bgpview.io/ip/"+args->target);
            break;
        case 7: // ip prefix
            url.setUrl("https://api.bgpview.io/prefix/"+args->target);
            break;
        case 8: // query
            url.setUrl("https://api.bgpview.io/search?query_term="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /* input type is a Query term, eg GOOGLE */
    if(args->inputDomain){
        url.setUrl("https://api.bgpview.io/search?query_term="+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /* input type is an ip-address, eg 1.1.1.1 */
    if(args->inputIp){
        url.setUrl("https://api.bgpview.io/ip/"+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    /* input type is an ASN number, eg 135340 */
    if(args->inputAsn){
        if(args->outputSubdomain || args->outputEmail){
            url.setUrl("https://api.bgpview.io/asn/"+args->target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args->outputIp){
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/prefixes");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args->outputAsn){
            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/peers");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/downstreams");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.bgpview.io/asn/"+args->target+"/upstreams");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Bgpview::replyFinished(QNetworkReply *reply){
    log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error())
        this->onError(reply);

    else
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            goto END;
        }

        /* all the json response from Bqpview api have the data object */
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = jsonReply.object()["data"].toObject();

        if(args->outputSubdomain){
            if(args->inputAsn){
                QString domain = data["website"].toString();
                domain = domain.remove(0, 8).remove("/");
                emit subdomain(domain);
                log.resultsCount++;
            }
            goto END;
        }

        if(args->outputAsn){
            if(args->inputAsn){
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
                goto END;
            }
            if(args->inputIp || args->inputDomain){
                QJsonArray asnList = data["asns"].toArray();
                foreach(const QJsonValue &value, asnList){
                    QString asnValue = QString::number(value.toObject()["asn"].toInt());
                    QString asnName = value.toObject()["name"].toString();
                    //...
                    emit asn(asnValue, asnName);
                    log.resultsCount++;
                }
                goto END;
            }
        }

        if(args->outputEmail){
            if(args->inputIp || args->inputAsn){
                QJsonArray emails = data["email_contacts"].toArray();
                foreach(const QJsonValue &value, emails){
                    emit email(value.toString());
                    log.resultsCount++;
                }
                goto END;
            }
            if(args->inputDomain){
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
                goto END;
            }
        }

        if(args->outputIp){
            if(args->inputIp){
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
                goto END;
            }
            if(args->inputAsn || args->inputDomain){
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
        }
    }

END:
    reply->deleteLater();
    activeRequests--;
    if(activeRequests == 0){
        //emit infoLog(log);
        emit quitThread();
    }
}
