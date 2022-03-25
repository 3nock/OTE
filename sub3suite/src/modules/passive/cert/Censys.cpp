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
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_CENSYS;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Censys::replyFinishedRawJson);
    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Censys::replyFinishedSSL);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Censys::replyFinishedSubdomain);

    /* getting api key */
    m_uid = APIKEY.value("censys_id").toString();
    m_key = APIKEY.value("censys_secret").toString();
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
    if(args.output_Raw){
        switch (args.raw_query_id) {
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

    if(args.input_Domain){
        if(args.output_URL || args.output_Hostname){
            url.setUrl("https://censys.io/api/v1/view/websites/"+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.input_IP){
        if(args.output_SSL || args.output_Hostname){
            url.setUrl("https://censys.io/api/v1/view/ipv4/"+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.input_SSL){
        if(args.output_SSL || args.output_Hostname){
            url.setUrl("https://censys.io/api/v1/view/certificates/"+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Censys::replyFinishedSSL(QNetworkReply *reply){
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
