#include "Censys.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ACCOUNT 0
#define VIEW_CERTIFICATE 1
#define VIEW_IPV4 2
#define VIEW_WEBSITES 3


Censys::Censys(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Censys";

    if(args.outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Censys::replyFinishedRawJson);
    if(args.outputSSLCert)
        connect(manager, &NetworkAccessManager::finished, this, &Censys::replyFinishedSSLCert);
    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Censys::replyFinishedSubdomain);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_uid = Config::generalConfig().value("censys_id").toString();
    m_key = Config::generalConfig().value("censys_secret").toString();
    Config::generalConfig().endGroup();
}
Censys::~Censys(){
    delete manager;
}

void Censys::start(){
    QNetworkRequest request;
    /* Basic Authentication */
    QString credentialsString = QString("%1:%2").arg(m_uid).arg(m_key);
    QByteArray credentialsArray = credentialsString.toLocal8Bit().toBase64();
    QString headerData = "Basic " + credentialsArray;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ACCOUNT:
            url.setUrl("https://censys.io/api/v1/account");
            break;
        case VIEW_CERTIFICATE:
            url.setUrl("https://censys.io/api/v1/view/certificates/"+target);
            break;
        case VIEW_IPV4:
            url.setUrl("https://censys.io/api/v1/view/ipv4/"+target);
            break;
        case VIEW_WEBSITES:
            url.setUrl("https://censys.io/api/v1/view/websites/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        if(args.outputUrl || args.outputSubdomain){
            url.setUrl("https://censys.io/api/v1/view/websites/"+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputIp){
        if(args.outputSSLCert || args.outputSubdomain){
            url.setUrl("https://censys.io/api/v1/view/ipv4/"+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputSSLCert){
        if(args.outputSSLCert || args.outputSubdomain){
            url.setUrl("https://censys.io/api/v1/view/certificates/"+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Censys::replyFinishedSSLCert(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    /* not yet implemented */

    end(reply);
}

void Censys::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    /* not yet implemented */

    end(reply);
}
