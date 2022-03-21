#include "LeakIX.h"
#include "src/utils/Config.h"

#define HOST 0
#define LEAK 1
#define SERVICES 2


LeakIX::LeakIX(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_LEAKIX;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &LeakIX::replyFinishedRawJson);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &LeakIX::replyFinishedAsn);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &LeakIX::replyFinishedSubdomain);
    if(args.outputSSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &LeakIX::replyFinishedSSL);

    /* getting api key */
    m_key = APIKEY.value(OSINT_MODULE_LEAKIX).toString();
    
}
LeakIX::~LeakIX(){
    delete manager;
}

void LeakIX::start(){
    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("api-key", m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case HOST:
            url.setUrl("https://leakix.net/host/"+target);
            break;
        case LEAK:
            url.setUrl("https://leakix.net/search?q="+target+"&scope=leak");
            break;
        case SERVICES:
            url.setUrl("https://leakix.net/search?q="+target+"&scope=service");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args.inputIp){
        if(args.outputAsn || args.outputSSL || args.outputSubdomain){
            url.setUrl("https://leakix.net/search?q="+target+"&scope=leak");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void LeakIX::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array()){
        emit resultSubdomain(value.toObject()["hostname"].toString());
        log.resultsCount++;
    }
    foreach(const QJsonValue &value, document.array()){
        QJsonObject domains = value.toObject()["certificate"].toObject()["domains"].toObject();
        foreach(const QJsonValue &domain, domains){
            emit resultSubdomain(domain.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void LeakIX::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array()){
        QString cert = value.toObject()["certificate"].toObject()["fingerprint"].toString();
        emit resultSSL(cert);
        log.resultsCount++;
    }

    end(reply);
}

void LeakIX::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array()){
        QJsonObject network = value.toObject()["network"].toObject();
        QString asn = QString::number(network["asn"].toInt());
        QString name = network["organization_name"].toString();
        emit resultASN(asn, name);
        log.resultsCount++;
    }
    end(reply);
}
