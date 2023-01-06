#include "Circl.h"
#include "src/utils/Config.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define PASSIVE_DNS 0
#define PASSIVE_SSL 1
#define PASSIVE_SSL_FETCH 2
#define PASSIVE_SSL_QUERY 3


Circl::Circl(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_CIRCL;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedRawJson);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedSubdomain);
    if(args.output_IP)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedIp);
    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Circl::replyFinishedSSL);

    /* get login credentials */
    m_id = gConfig.keys.Circl.user;
    m_password = gConfig.keys.Circl.pwd;
}
Circl::~Circl(){
    delete manager;
}

void Circl::start(){
    this->checkAPIKey(m_id);
    this->checkAPIKey(m_password);

    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case PASSIVE_DNS:
            url.setUrl("https://www.circl.lu/pdns/query/"+target+"?id="+m_id+"&pass="+m_password);
            break;
        case PASSIVE_SSL:
            url.setUrl("https://www.circl.lu/v2pssl/query/"+target+"?id="+m_id+"&pass="+m_password);
            break;
        case PASSIVE_SSL_FETCH:
            url.setUrl("https://www.circl.lu/v2pssl/cfetch/"+target+"?id="+m_id+"&pass="+m_password);
            break;
        case PASSIVE_SSL_QUERY:
            url.setUrl("https://www.circl.lu/v2pssl/cquery/"+target+"?id="+m_id+"&pass="+m_password);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_IP || args.output_Hostname){
            url.setUrl("https://www.circl.lu/pdns/query/"+target+"?id="+m_id+"&pass="+m_password);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_IP){
        if(args.output_IP || args.output_Hostname){
            url.setUrl("https://www.circl.lu/pdns/query/"+target+"?id="+m_id+"&pass="+m_password);
            request.setAttribute(QNetworkRequest::User, PASSIVE_DNS);
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_SSL){
            url.setUrl("https://www.circl.lu/pdns/query/"+target+"?id="+m_id+"&pass="+m_password);
            request.setAttribute(QNetworkRequest::User, PASSIVE_SSL);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_SSL){
        if(args.output_IP){
            url.setUrl("https://www.circl.lu/v2pssl/cquery/"+target+"?id="+m_id+"&pass="+m_password);
            request.setAttribute(QNetworkRequest::User, PASSIVE_SSL_QUERY);
            request.setUrl(url);
            manager->get(request);
        }
    }
}

void Circl::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jsonArray = document.array();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_DNS:
        foreach(const QJsonValue &value, jsonArray){
            QString rrtype = value.toObject()["rrtype"].toString();
            QString rdata = value.toObject()["rdata"].toString();

            if(rrtype == "NS"){
                emit resultNS(rdata);
                log.resultsCount++;
            }
            if(rrtype == "MX"){
                emit resultMX(rdata);
                log.resultsCount++;
            }
            if(rrtype == "CNAME"){
                emit resultCNAME(rdata);
                log.resultsCount++;
            }
        }
    }

    this->end(reply);
}

void Circl::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jsonArray = document.array();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_DNS:
        foreach(const QJsonValue &value, jsonArray){
            QString rrtype = value.toObject()["rrtype"].toString();
            QString rdata = value.toObject()["rdata"].toString();

            if(rrtype == "A"){
                emit resultA(rdata);
                log.resultsCount++;
            }
            if(rrtype == "AAAA"){
                emit resultAAAA(rdata);
                log.resultsCount++;
            }
        }
        break;
    }

    this->end(reply);
}

void Circl::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = document.object();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case PASSIVE_SSL:
        foreach(const QString &key, jsonObject.keys()){
            QJsonArray certificates = jsonObject[key].toObject()["certificates"].toArray();
            foreach(const QJsonValue &value, certificates)
            {
                emit resultSSL(value.toString());
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}
