#include "Bgpview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#define ASN_ 0
#define ASN_DOWNSTREAM 1
#define ASN_IXS 2
#define ASN_PEERS 3
#define ASN_PREFIXES 4
#define ASN_UPSTREAMS 5
#define IP 6
#define IP_PREFIXES 7
#define QUERY 8


Bgpview::Bgpview(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_BGPVIEW;

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedRawJson);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedAsn);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedEmail);
    if(args.outputCidr)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedCidr);
    if(args.outputInfoCidr)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedInfoCidr);
    if(args.outputInfoAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedInfoAsn);
    if(args.outputInfoAsnPeers)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedInfoAsnPeers);
    if(args.outputInfoAsnPrefixes)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bgpview::replyFinishedInfoAsnPrefixes);
}
Bgpview::~Bgpview(){
    delete manager;
}

void Bgpview::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");

    QUrl url;
    /* for raw output */
    if(args.outputRaw){
        switch(args.rawOption){
        case ASN_:
            url.setUrl("https://api.bgpview.io/asn/"+target);
            break;
        case ASN_DOWNSTREAM:
            url.setUrl("https://api.bgpview.io/asn/"+target+"/downstreams");
            break;
        case ASN_IXS:
            url.setUrl("https://api.bgpview.io/asn/"+target+"/ixs");
            break;
        case ASN_PEERS:
            url.setUrl("https://api.bgpview.io/asn/"+target+"/peers");
            break;
        case ASN_PREFIXES:
            url.setUrl("https://api.bgpview.io/asn/"+target+"/prefixes");
            break;
        case ASN_UPSTREAMS:
            url.setUrl("https://api.bgpview.io/asn/"+target+"/upstreams");
            break;
        case IP:
            url.setUrl("https://api.bgpview.io/ip/"+target);
            break;
        case IP_PREFIXES:
            url.setUrl("https://api.bgpview.io/prefix/"+target);
            break;
        case QUERY:
            url.setUrl("https://api.bgpview.io/search?query_term="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /* for info output */

    if(args.outputInfoCidr){
        url.setUrl("https://api.bgpview.io/prefix/"+target);
        request.setAttribute(QNetworkRequest::User, IP_PREFIXES);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.outputInfoAsn){
        url.setUrl("https://api.bgpview.io/asn/"+target);
        request.setAttribute(QNetworkRequest::User, ASN_);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.outputInfoAsnPeers){
        url.setUrl("https://api.bgpview.io/asn/"+target+"/peers");
        request.setAttribute(QNetworkRequest::User, ASN_PEERS);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.outputInfoAsnPrefixes){
        url.setUrl("https://api.bgpview.io/asn/"+target+"/prefixes");
        request.setAttribute(QNetworkRequest::User, ASN_PREFIXES);
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    /* for normal output */
    if(args.inputQueryTerm){
        if(args.outputAsn || args.outputCidr || args.outputEmail){
            url.setUrl("https://api.bgpview.io/search?query_term="+target);
            request.setAttribute(QNetworkRequest::User, QUERY);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputIp){
        if(args.outputAsn || args.outputIp || args.outputEmail){
            url.setUrl("https://api.bgpview.io/ip/"+target);
            request.setAttribute(QNetworkRequest::User, IP);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputAsn){
        if(args.outputSubdomain){
            url.setUrl("https://api.bgpview.io/asn/"+target);
            request.setAttribute(QNetworkRequest::User, ASN_);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputEmail){
            url.setUrl("https://api.bgpview.io/asn/"+target);
            request.setAttribute(QNetworkRequest::User, ASN_);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputIp){
            url.setUrl("https://api.bgpview.io/asn/"+target+"/prefixes");
            request.setAttribute(QNetworkRequest::User, ASN_PREFIXES);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputCidr){
            url.setUrl("https://api.bgpview.io/asn/"+target+"/prefixes");
            request.setAttribute(QNetworkRequest::User, ASN_PREFIXES);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputAsn){
            url.setUrl("https://api.bgpview.io/asn/"+target+"/peers");
            request.setAttribute(QNetworkRequest::User, ASN_PEERS);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
    }

    if(args.inputCidr){
        if(args.outputAsn || args.outputEmail){
            url.setUrl("https://api.bgpview.io/prefix/"+target);
            request.setAttribute(QNetworkRequest::User, IP_PREFIXES);
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

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP:
        foreach(const QJsonValue &value, data["prefixes"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultIp(ip);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["related_prefixes"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultIp(ip);
            log.resultsCount++;
        }
        break;

    case ASN_PREFIXES:
        foreach(const QJsonValue &value, data["ipv4_prefixes"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultA(ip);
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["ipv6_prefixes"].toArray()){
            QString ip = value.toObject()["ip"].toString();
            emit resultAAAA(ip);
            log.resultsCount++;
        }
        break;
    }

    end(reply);
}

void Bgpview::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP:
    case QUERY:
    case IP_PREFIXES:
        foreach(const QJsonValue &value, data["asns"].toArray()){
            QString asn = QString::number(value.toObject()["asn"].toInt());
            QString name = value.toObject()["name"].toString();

            emit resultASN(asn, name);
            log.resultsCount++;
        }
        break;

    case ASN_PEERS:
        foreach(const QString &key, data.keys()){
            foreach(const QJsonValue &value, data[key].toArray()){
                QString asn = QString::number(value.toObject()["asn"].toInt());
                QString name = value.toObject()["name"].toString();

                emit resultASN(asn, name);
                log.resultsCount++;
            }
        }
        break;
    }

    end(reply);
}

void Bgpview::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP:
    case ASN_:
    case IP_PREFIXES:
        foreach(const QJsonValue &value, data["email_contacts"].toArray()){
            emit resultEmail(value.toString());
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["abuse_contacts"].toArray()){
            emit resultEmail(value.toString());
            log.resultsCount++;
        }
        break;

    case QUERY:
        foreach(const QJsonValue &asn, data["asns"].toArray()){
            foreach(const QJsonValue &email, asn.toObject()["email_contacts"].toArray()){
                emit resultEmail(email.toString());
                log.resultsCount++;
            }
            foreach(const QJsonValue &email, asn.toObject()["abuse_contacts"].toArray()){
                emit resultEmail(email.toString());
                log.resultsCount++;
            }
        }
        foreach(const QJsonValue &asn, data["ipv4_prefixes"].toArray()){
            foreach(const QJsonValue &email, asn.toObject()["email_contacts"].toArray()){
                emit resultEmail(email.toString());
                log.resultsCount++;
            }
            foreach(const QJsonValue &email, asn.toObject()["abuse_contacts"].toArray()){
                emit resultEmail(email.toString());
                log.resultsCount++;
            }
        }
        foreach(const QJsonValue &asn, data["ipv6_prefixes"].toArray()){
            foreach(const QJsonValue &email, asn.toObject()["email_contacts"].toArray()){
                emit resultEmail(email.toString());
                log.resultsCount++;
            }
            foreach(const QJsonValue &email, asn.toObject()["abuse_contacts"].toArray()){
                emit resultEmail(email.toString());
                log.resultsCount++;
            }
        }
    }

    end(reply);
}

void Bgpview::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASN_:
        QString subdomain = data["website"].toString();
        subdomain = subdomain.remove(0, 8).remove("/");
        emit resultSubdomain(subdomain);
        log.resultsCount++;
        break;
    }

    end(reply);
}

void Bgpview::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case QUERY:
    case ASN_PREFIXES:
        foreach(const QJsonValue &value, data["ipv4_prefixes"].toArray()){
            emit resultCidr(value.toObject()["prefix"].toString());
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["ipv6_prefixes"].toArray()){
            emit resultCidr(value.toObject()["prefix"].toString());
            log.resultsCount++;
        }
        break;
    }


    end(reply);
}

///
/// Info Results...
///

void Bgpview::replyFinishedInfoAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASN_:
        s3s_struct::ASN asn;
        asn.asn = target;

        /* general info */
        asn.info_asn = QString::number(data["asn"].toInt());
        asn.info_name = data["name"].toString();
        asn.info_description = data["description_short"].toString();
        asn.info_country = data["country_code"].toString();
        asn.info_website = data["website"].toString();
        // owner's address...
        QString address = "";
        foreach(const QJsonValue &value, data["owner_address"].toArray())
            address.append(value.toString()+", ");
        asn.info_ownerAddress = address;

        /* email contacts */
        foreach(const QJsonValue &value, data["email_contacts"].toArray())
            asn.emailcontacts.insert(value.toString());

        /* abuse contacts */
        foreach(const QJsonValue &value, data["abuse_contacts"].toArray())
            asn.abusecontacts.insert(value.toString());

        /* rir allocation */
        QJsonObject rirAllocation = data["rir_allocation"].toObject();
        asn.rir_name = rirAllocation["rir_name"].toString();
        asn.rir_country = rirAllocation["country_code"].toString();
        asn.rir_dateallocated = rirAllocation["date_allocated"].toString();

        /* sending results */
        emit infoASN(asn);
        break;
    }

    end(reply);
}

void Bgpview::replyFinishedInfoAsnPeers(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASN_PEERS:
        s3s_struct::ASN asn;

        /* target */
        asn.asn = target;

        foreach(const QJsonValue &value, data["ipv4_peers"].toArray())
            asn.peers.insert(QString::number(value.toObject()["asn"].toInt()));

        foreach(const QJsonValue &value, data["ipv6_peers"].toArray())
            asn.peers.insert(QString::number(value.toObject()["asn"].toInt()));

        /* sending results */
        emit infoASN(asn);
    }

    end(reply);
}

void Bgpview::replyFinishedInfoAsnPrefixes(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case ASN_PREFIXES:
        s3s_struct::ASN asn;

        /* target */
        asn.asn = target;

        foreach(const QJsonValue &value, data["ipv4_prefixes"].toArray())
            asn.prefixes.insert(value.toObject()["prefix"].toString());

        foreach(const QJsonValue &value, data["ipv6_prefixes"].toArray())
            asn.prefixes.insert(value.toObject()["prefix"].toString());

        /* sending results */
        emit infoASN(asn);
    }

    end(reply);
}

void Bgpview::replyFinishedInfoCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    switch (reply->property(REQUEST_TYPE).toInt())
    {
    case IP_PREFIXES:
        s3s_struct::CIDR cidr;
        cidr.cidr = target;

        /* info */
        cidr.info_prefix = data["prefix"].toString();
        cidr.info_ip = data["ip"].toString();
        cidr.info_cidr = QString::number(data["cidr"].toInt());
        cidr.info_name = data["name"].toString();
        cidr.info_description = data["description_short"].toString();
        cidr.info_country = data["country_code"].toString();
        cidr.info_website = data["website"].toString();
        // owner's address...
        QString address = "";
        foreach(const QJsonValue &value, data["owner_address"].toArray())
            address.append(value.toString()+", ");
        cidr.info_ownerAddress = address;

        /* email contacts */
        foreach(const QJsonValue &value, data["email_contacts"].toArray())
            cidr.emailcontacts.insert(value.toString());

        /* abuse contacts */
        foreach(const QJsonValue &value, data["abuse_contacts"].toArray())
            cidr.abusecontacts.insert(value.toString());

        /* rir allocation */
        QJsonObject rirAllocation = data["rir_allocation"].toObject();
        cidr.rir_name = rirAllocation["rir_name"].toString();
        cidr.rir_country = rirAllocation["country_code"].toString();
        cidr.rir_prefix = rirAllocation["prefix"].toString();
        cidr.rir_dateallocated = rirAllocation["date_allocated"].toString();

        /* asns */
        foreach(const QJsonValue &value, data["asns"].toArray()){
            QString asn = QString::number(value.toObject()["asn"].toInt());
            cidr.asns.insert(asn);
        }

        /* sending results */
        emit infoCIDR(cidr);
    }

    end(reply);
}
