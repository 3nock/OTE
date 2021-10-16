#include "BinaryEdge.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


BinaryEdge::BinaryEdge(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &BinaryEdge::replyFinished);
    ///
    /// getting the api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("binaryedge").toString();
    Config::generalConfig().endGroup();
}
BinaryEdge::~BinaryEdge(){
    delete manager;
}

void BinaryEdge::start(){
    QNetworkRequest request;
    QUrl url;

    if(args->raw){
        if(args->option == "host ip")
            url.setUrl("https://api.binaryedge.io/v2/query/ip/"+args->target);
        if(args->option == "host historical")
            url.setUrl("https://api.binaryedge.io/v2/query/ip/historical/"+args->target);
        if(args->option == "host search")
            url.setUrl("https://api.binaryedge.io/v2/query/search?query="+args->target);
        if(args->option == "host search stats")
            url.setUrl("https://api.binaryedge.io/v2/query/search/stats?query="+args->target);
        if(args->option == "domains subdomain")
            url.setUrl("https://api.binaryedge.io/v2/query/domains/subdomain/"+args->target);
        if(args->option == "domains dns")
            url.setUrl("https://api.binaryedge.io/v2/query/domains/dns/"+args->target);
        if(args->option == "domains ip")
            url.setUrl("https://api.binaryedge.io/v2/query/domains/ip/"+args->target);
        if(args->option == "domains search")
            url.setUrl("https://api.binaryedge.io/v2/query/domains/search?query="+args->target);
        if(args->option == "domains enumeration")
            url.setUrl("https://api.binaryedge.io/v2/query/domains/enumeration/"+args->target);
        if(args->option == "domains homoglyphs")
            url.setUrl("https://api.binaryedge.io/v2/query/domains/homoglyphs/"+args->target);
    }else{
        url.setUrl("https://api.binaryedge.io/v2/query/domains/subdomain/"+args->target+"?page="+QString::number(m_page));
    }

    request.setUrl(url);
    request.setRawHeader("X-KEY", m_key.toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    manager->get(request);
}

void BinaryEdge::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        if(args->raw){
            emit rawResults(reply->readAll());
            reply->deleteLater();
            emit quitThread();
            return;
        }
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        /*
         * QString currentPage = jsonObject["page"].toString();
         * QString pageSize = jsonObject["pagesize"].toString();
         * QString totalSubdomains = jsonObject["total"].toString();
         *
         */
        QJsonArray subdomainList = jsonObject["events"].toArray();
        ///
        /// if the end of the list exit...
        ///
        if(subdomainList.isEmpty())
            goto END;
        ///
        /// if not get subdomainIp subdomains...
        ///
        foreach(const QJsonValue &value, subdomainList)
            emit subdomain(value.toString());
        ///
        /// next query...
        ///
        m_page++;
        QNetworkRequest request;
        QUrl url("https://api.binaryedge.io/v2/query/domains/subdomain/"+args->target+"?page="+QString::number(m_page));
        request.setUrl(url);
        request.setRawHeader("X-KEY", m_key.toUtf8());
        request.setRawHeader("Content-Type", "application/json");
        manager->get(request);
        ///
        /// cleanup and exit function...
        ///
        reply->deleteLater();
        return;
    }
    else{
        emit errorLog(reply->errorString());
    }

END:
    reply->deleteLater();
    emit quitThread();
}
