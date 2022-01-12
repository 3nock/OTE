#include "Dnslytics.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define ACCOUNTINFO 0
#define ASINFO 1
#define DOMAINSEARCH 2
#define DOMAINTYPOS 3
#define HOSTINGHISTORY 4
#define IP2ASN 5
#define IPINFO 6
#define REVERSEADSENSE 7
#define REVERSEGANALYTICS 8
#define REVERSEIP 9
#define REVERSEMX 10
#define REVERSENS 11
#define SUBNETINFO 12


Dnslytics::Dnslytics(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = OSINT_MODULE_DNSLYTICS;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedAsn);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedSubdomainIp);
    if(args.outputCidr)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedCidr);
    if(args.outputInfoIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedInfoIp);
    if(args.outputInfoMX)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedInfoMX);
    if(args.outputInfoNS)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Dnslytics::replyFinishedInfoNS);

    /* getting api key... */
    
    m_key = APIKEY.value(OSINT_MODULE_DNSLYTICS).toString();
    
}
Dnslytics::~Dnslytics(){
    delete manager;
}

void Dnslytics::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case ACCOUNTINFO:
            url.setUrl("https://api.dnslytics.net/v1/accountinfo?apikey="+m_key);
            break;
        case ASINFO:
            url.setUrl("https://api.dnslytics.net/v1/asinfo/"+target+"/summary?apikey="+m_key);
            break;
        case DOMAINSEARCH:
            url.setUrl("https://api.dnslytics.net/v1/domainsearch/"+target+"?apikey="+m_key);
            break;
        case DOMAINTYPOS:
            url.setUrl("https://api.dnslytics.net/v1/domaintypos/"+target+"?apikey="+m_key);
            break;
        case HOSTINGHISTORY:
            url.setUrl("https://api.dnslytics.net/v1/hostinghistory/"+target+"?apikey="+m_key);
            break;
        case IPINFO:
            url.setUrl("https://api.dnslytics.net/v1/ipinfo/"+target+"?apikey="+m_key);
            break;
        case IP2ASN:
            url.setUrl("https://freeapi.dnslytics.net/v1/ip2asn/"+target);
            break;
        case REVERSEADSENSE:
            url.setUrl("https://api.dnslytics.net/v1/reverseadsense/"+target+"?apikey="+m_key);
            break;
        case REVERSEGANALYTICS:
            url.setUrl("https://api.dnslytics.net/v1/reverseganalytics/"+target+"?apikey="+m_key);
            break;
        case REVERSEIP:
            url.setUrl("https://api.dnslytics.net/v1/reverseip/"+target+"?apikey="+m_key);
            break;
        case REVERSEMX:
            url.setUrl("https://api.dnslytics.net/v1/reversemx/"+target+"?apikey="+m_key);
            break;
        case REVERSENS:
            url.setUrl("https://api.dnslytics.net/v1/reversens/"+target+"?apikey="+m_key);
            break;
        case SUBNETINFO:
            url.setUrl("https://api.dnslytics.net/v1/subnetinfo/"+target+"/summary?apikey="+m_key);
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    ///
    /// for info output...
    ///
    if(args.outputInfoIp){
        url.setUrl("https://api.dnslytics.net/v1/reverseip/"+target+"?apikey="+m_key);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.outputInfoNS){
        url.setUrl("https://api.dnslytics.net/v1/reversens/"+target+"?apikey="+m_key);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.outputInfoMX){
        url.setUrl("https://api.dnslytics.net/v1/reversemx/"+target+"?apikey="+m_key);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    ///
    /// for osint output...
    ///
    if(args.inputDomain){
        if(args.outputSubdomain){
            url.setUrl("https://api.dnslytics.net/v1/domainsearch/"+target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, DOMAINSEARCH);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputIp){
            url.setUrl("https://api.dnslytics.net/v1/hostinghistory/"+target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, HOSTINGHISTORY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputIp){
        if(args.outputSubdomain){
            url.setUrl("https://api.dnslytics.net/v1/reverseip/"+target+"?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, REVERSEIP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputAsn || args.outputCidr){
            url.setUrl("https://freeapi.dnslytics.net/v1/ip2asn/"+target);
            request.setAttribute(QNetworkRequest::User, IP2ASN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputAsn){
        if(args.outputIp || args.outputAsn || args.outputSubdomain || args.outputSubdomainIp || args.outputCidr){
            url.setUrl("https://api.dnslytics.net/v1/asinfo/"+target+"/summary?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, ASINFO);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.inputCidr){
        if(args.outputIp || args.outputAsn || args.outputSubdomain || args.outputSubdomainIp || args.outputCidr){
            url.setUrl("https://api.dnslytics.net/v1/subnetinfo/"+target+"/summary?apikey="+m_key);
            request.setAttribute(QNetworkRequest::User, SUBNETINFO);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Dnslytics::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (QUERY_TYPE) {
    case ASINFO:
    case SUBNETINFO:
    {
        /* mxrecords */
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString mxrecord = value.toObject()["mxrecord"].toString();
            emit resultMX(mxrecord);
            log.resultsCount++;
        }
        /* nsrecords */
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString nsrecord = value.toObject()["nsrecord"].toString();
            emit resultNS(nsrecord);
            log.resultsCount++;
        }
        /* domains */
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            emit resultSubdomain(domain);
            log.resultsCount++;
        }
    }
    break;
    case DOMAINSEARCH:
    {
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            emit resultSubdomain(domain);
            log.resultsCount++;
        }
    }
    break;
    case REVERSEIP:
    {
        foreach(const QJsonValue &value, data["domains"].toArray()){
            emit resultSubdomain(value.toString());
            log.resultsCount++;
        }
    }
    }

    end(reply);
}

void Dnslytics::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (QUERY_TYPE) {
    case ASINFO:
    case SUBNETINFO:
        /* mxrecords */
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString mxrecord = value.toObject()["mxrecord"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultSubdomainIp(mxrecord, ipv4);
            log.resultsCount++;
        }
        /* nsrecords */
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString nsrecord = value.toObject()["nsrecord"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultSubdomainIp(nsrecord, ipv4);
            log.resultsCount++;
        }
        /* domains */
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString domain = value.toObject()["domain"].toString();
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultSubdomainIp(domain, ipv4);
            log.resultsCount++;
        }
    }

    end(reply);
}

void Dnslytics::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (QUERY_TYPE) {
    case ASINFO:
    {
        /* peersv4 */
        foreach(const QJsonValue &value, data["peersv4"].toArray()){
            QString asn = QString::number(value.toObject()["aspeer"].toInt());
            QString name = value.toObject()["shortname"].toString();
            emit resultASN(asn, name);
            log.resultsCount++;
        }
        /* peersv6 */
        foreach(const QJsonValue &value, data["peersv6"].toArray()){
            QString asn = QString::number(value.toObject()["aspeer"].toInt());
            QString name = value.toObject()["shortname"].toString();
            emit resultASN(asn, name);
            log.resultsCount++;
        }
    }
    break;
    case SUBNETINFO:
    {
        foreach(const QJsonValue &value, data["routes"].toArray()){
            QString asn = QString::number(value.toObject()["asn"].toInt());
            QString name = value.toObject()["shortname"].toString();
            emit resultASN(asn, name);
            log.resultsCount++;
        }
    }
    break;
    case IP2ASN:
    {
        QJsonObject mainObj = document.object();
        QString asn = QString::number(mainObj["asn"].toInt());
        QString name = mainObj["shortname"].toString();

        emit resultASN(asn, name);
        log.resultsCount++;
    }
    }

    end(reply);
}

void Dnslytics::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (QUERY_TYPE) {
    case ASINFO:
    case SUBNETINFO:
    {
        /* mxrecords */
        foreach(const QJsonValue &value, data["mxrecords"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultA(ipv4);
            log.resultsCount++;
        }
        /* nsrecords */
        foreach(const QJsonValue &value, data["nsrecords"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultA(ipv4);
            log.resultsCount++;
        }
        /* domains */
        foreach(const QJsonValue &value, data["domains"].toArray()){
            QString ipv4 = value.toObject()["ipv4"].toString();
            emit resultA(ipv4);
            log.resultsCount++;
        }
    }
    break;
    case HOSTINGHISTORY:
    {
        /* ipv6 */
        foreach(const QJsonValue &value, data["ipv6"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultAAAA(ip);
            log.resultsCount++;
        }
        /* ipv4 */
        foreach(const QJsonValue &value, data["ipv4"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultA(ip);
            log.resultsCount++;
        }
    }
    }

    end(reply);
}

void Dnslytics::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (QUERY_TYPE) {
    case ASINFO:
    {
        /* prefixesv4 */
        foreach(const QJsonValue &value, data["prefixesv4"].toArray()){
            QString Prefix = value.toObject()["prefix"].toString();
            emit resultCidr(Prefix);
            log.resultsCount++;
        }
        /* prefixesv6 */
        foreach(const QJsonValue &value, data["prefixesv6"].toArray()){
            QString Prefix = value.toObject()["prefix"].toString();
            emit resultCidr(Prefix);
            log.resultsCount++;
        }
    }
    break;
    case SUBNETINFO:
    {
        foreach(const QJsonValue &value, data["routes"].toArray()){
            QString cidr = value.toObject()["cidr"].toString();
            emit resultCidr(cidr);
            log.resultsCount++;
        }
    }
    break;
    case IP2ASN:
    {
        QJsonObject mainObj = document.object();
        QString cidr = mainObj["cidr"].toString();

        emit resultCidr(cidr);
        log.resultsCount++;
    }
    }

    end(reply);
}

///
/// For info...
///

void Dnslytics::replyFinishedInfoMX(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    /* initiate the struct */
    MXModelStruct mxModelStruct;

    /* append the domains */
    foreach(const QJsonValue &value, data["domains"].toArray()){
        mxModelStruct.domains.insert(value.toString());
    }

    /* send the structure */
    emit infoMX(mxModelStruct);

    end(reply);
}

void Dnslytics::replyFinishedInfoNS(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    /* initiate the struct */
    NSModelStruct nsModelStruct;

    /* append the domains */
    foreach(const QJsonValue &value, data["domains"].toArray()){
        nsModelStruct.domains.insert(value.toString());
    }

    /* send the structure */
    emit infoNS(nsModelStruct);

    end(reply);
}

void Dnslytics::replyFinishedInfoIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    /* initiate the struct */
    IpModelStruct ipModelStruct;

    /* append the domains */
    foreach(const QJsonValue &value, data["domains"].toArray()){
        ipModelStruct.domains.insert(value.toString());
    }

    /* send the structure */
    emit infoIp(ipModelStruct);

    end(reply);
}
