#include "BinaryEdge.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


BinaryEdge::BinaryEdge(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &BinaryEdge::replyFinished);
    //...
    log.moduleName = "BinaryEdge";
    log.resultsCount = 0;

    /* getting the api-key... */
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("binaryedge").toString();
    Config::generalConfig().endGroup();
}
BinaryEdge::~BinaryEdge(){
    delete manager;
}

void BinaryEdge::start(){
    QNetworkRequest request;
    request.setRawHeader("X-KEY", m_key.toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    QUrl url;

    if(args->raw){
        switch (args->rawOption) {
        case 0: // domain dns
            url.setUrl("https://api.binaryedge.io/v2/query/domains/dns/"+args->target);
            break;
        case 1: // domain enumeration
            url.setUrl("https://api.binaryedge.io/v2/query/domains/enumeration/"+args->target);
            break;
        case 2: //domain hymoglyphs
            url.setUrl("https://api.binaryedge.io/v2/query/domains/homoglyphs/"+args->target);
            break;
        case 3: // domain ip
            url.setUrl("https://api.binaryedge.io/v2/query/domains/ip/"+args->target);
            break;
        case 4: // domain search
            url.setUrl("https://api.binaryedge.io/v2/query/domains/search?query="+args->target);
            break;
        case 5: // domain subdomains
            url.setUrl("https://api.binaryedge.io/v2/query/domains/subdomain/"+args->target);
            break;
        case 6: // host historical
            url.setUrl("https://api.binaryedge.io/v2/query/ip/historical/"+args->target);
            break;
        case 7: // host ip
            url.setUrl("https://api.binaryedge.io/v2/query/ip/"+args->target);
            break;
        case 8: // host search
            url.setUrl("https://api.binaryedge.io/v2/query/search?query="+args->target);
            break;
        case 9: //host search stats
            url.setUrl("https://api.binaryedge.io/v2/query/search/stats?query="+args->target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /* if the input is an ip-address */
    if(args->inputIp){
        url.setUrl("https://api.binaryedge.io/v2/query/ip/"+args->target);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /* if input is a domain name */
    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.binaryedge.io/v2/query/domains/subdomain/"+args->target+"?page="+QString::number(m_page));
            request.setUrl(url);
            manager->get(request);
            activeRequests++;

            url.setUrl("https://api.binaryedge.io/v2/query/domains/dns/"+args->target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args->outputSubdomainIp || args->outputIp){
            url.setUrl("https://api.binaryedge.io/v2/query/domains/dns/"+args->target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void BinaryEdge::replyFinished(QNetworkReply *reply){
    log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error())
        this->onError(reply);
    else
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            goto END;
        }

        /* all the json replies results are in the events array */
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonArray events = jsonReply.object()["events"].toArray();

        /* for subdomain results */
        if(args->outputSubdomain){
            if(args->inputDomain){
                foreach(const QJsonValue &value, events){
                    if(value.isString()){
                        emit subdomain(value.toString());
                        log.resultsCount++;
                    }
                    if(value.isObject()){
                        QString domain = value.toObject()["domain"].toString();
                        emit subdomain(domain);
                        log.resultsCount++;

                        foreach(const QJsonValue &value, value.toObject()["NS"].toArray()){
                            emit NS(value.toString());
                            log.resultsCount++;
                        }
                        foreach(const QJsonValue &value, value.toObject()["MX"].toArray()){
                            emit MX(value.toString());
                            log.resultsCount++;
                        }
                    }
                }
                goto END;
            }

            if(args->inputIp){
                foreach(const QJsonValue &value, events){
                    QString domain = value.toObject()["domain"].toString();
                    emit subdomain(domain);
                    log.resultsCount++;

                    foreach(const QJsonValue &value, value.toObject()["NS"].toArray()){
                        emit NS(value.toString());
                        log.resultsCount++;
                    }
                    foreach(const QJsonValue &value, value.toObject()["MX"].toArray()){
                        emit MX(value.toString());
                        log.resultsCount++;
                    }
                }
                goto END;
            }
        }

        /* for subdomainIp resuts */
        if(args->outputSubdomainIp){
            foreach(const QJsonValue &value, events){
                QString domain = value.toObject()["domain"].toString();
                QString A = value.toObject()["A"].toArray().at(0).toString();
                QString AAAA = value.toObject()["AAAA"].toArray().at(0).toString();
                if(!A.isEmpty()){
                    emit subdomainIp(domain, A);
                    log.resultsCount++;
                }
                if(!AAAA.isEmpty()){
                    emit subdomainIp(domain, AAAA);
                    log.resultsCount++;
                }
            }
            goto END;
        }

        /* for ip results... */
        if(args->outputIp){
            foreach(const QJsonValue &value, events){
                QString A = value.toObject()["A"].toArray().at(0).toString();
                QString AAAA = value.toObject()["AAAA"].toArray().at(0).toString();
                if(!A.isEmpty()){
                    emit ipA(A);
                    log.resultsCount++;
                }
                if(!AAAA.isEmpty()){
                    emit ipAAAA(AAAA);
                    log.resultsCount++;
                }
            }
            goto END;
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
