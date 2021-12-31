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


Bgpview::Bgpview(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
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
        case ASN:
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
        request.setAttribute(QNetworkRequest::User, ASN);
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
            request.setAttribute(QNetworkRequest::User, ASN);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
            return;
        }
        if(args.outputEmail){
            url.setUrl("https://api.bgpview.io/asn/"+target);
            request.setAttribute(QNetworkRequest::User, ASN);
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(QUERY_TYPE == IP){
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
    }

    /* asn prefixes and query return results in similar format */
    if(QUERY_TYPE == ASN_PREFIXES){
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
    }
    end(reply);
}

void Bgpview::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    /* asns from ip prefixes */
    if(QUERY_TYPE == IP_PREFIXES){
        foreach(const QJsonValue &value, data["asns"].toArray()){
            QString asn = QString::number(value.toObject()["asn"].toInt());
            QString name = value.toObject()["name"].toString();

            emit resultASN(asn, name);
            log.resultsCount++;
        }
    }

    /* asn peers, asn upstreams and asn downstreams return results in similar format */
    if(QUERY_TYPE == ASN_PEERS){
        QStringList keys = data.keys();
        foreach(const QString &key, keys){
            foreach(const QJsonValue &value, data[key].toArray()){
                QString asn = QString::number(value.toObject()["asn"].toInt());
                QString name = value.toObject()["name"].toString();

                emit resultASN(asn, name);
                log.resultsCount++;
            }
        }
    }

    /* ip and query return results in similar format */
    if(QUERY_TYPE == IP){
        foreach(const QJsonValue &value, data["asns"].toArray()){
            QString asn = QString::number(value.toObject()["asn"].toInt());
            QString name = value.toObject()["name"].toString();

            emit resultASN(asn, name);
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    /* ip, asn & Cidr results contains email contacts in same format */
    if(QUERY_TYPE == IP || QUERY_TYPE == ASN || QUERY_TYPE == IP_PREFIXES){
        foreach(const QJsonValue &value, data["email_contacts"].toArray())
        {
            emit resultEmail(value.toString());
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(QUERY_TYPE == ASN){
        QString subdomain = data["website"].toString();
        subdomain = subdomain.remove(0, 8).remove("/");
        emit resultSubdomain(subdomain);
        log.resultsCount++;
    }
    end(reply);
}

void Bgpview::replyFinishedCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(QUERY_TYPE == ASN_PREFIXES){
        foreach(const QJsonValue &value, data["ipv4_prefixes"].toArray()){
            emit resultCidr(value.toObject()["prefix"].toString());
            log.resultsCount++;
        }
        foreach(const QJsonValue &value, data["ipv6_prefixes"].toArray()){
            emit resultCidr(value.toObject()["prefix"].toString());
            log.resultsCount++;
        }
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

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(QUERY_TYPE == ASN)
    {
        AsModelStruct asModel;

        /* general info */
        asModel.info_asn = QString::number(data["asn"].toInt());
        asModel.info_name = data["name"].toString();
        asModel.info_description = data["description_short"].toString();
        asModel.info_country = data["country_code"].toString();
        asModel.info_website = data["website"].toString();
        // owner's address...
        QString address = "";
        foreach(const QJsonValue &value, data["owner_address"].toArray())
            address.append(value.toString()+", ");
        asModel.info_ownerAddress = address;

        /* email contacts */
        foreach(const QJsonValue &value, data["email_contacts"].toArray())
            asModel.emailcontacts.insert(value.toString());

        /* abuse contacts */
        foreach(const QJsonValue &value, data["abuse_contacts"].toArray())
            asModel.abusecontacts.insert(value.toString());

        /* rir allocation */
        QJsonObject rirAllocation = data["rir_allocation"].toObject();
        asModel.rir_name = rirAllocation["rir_name"].toString();
        asModel.rir_country = rirAllocation["country_code"].toString();
        asModel.rir_dateallocated = rirAllocation["date_allocated"].toString();

        /* sending results */
        emit infoASN(asModel);
    }

    end(reply);
}

void Bgpview::replyFinishedInfoAsnPeers(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(QUERY_TYPE == ASN_PEERS){
        AsModelStruct asModel;

        foreach(const QJsonValue &value, data["ipv4_peers"].toArray())
            asModel.peers.insert(QString::number(value.toObject()["asn"].toInt()));

        foreach(const QJsonValue &value, data["ipv6_peers"].toArray())
            asModel.peers.insert(QString::number(value.toObject()["asn"].toInt()));

        /* sending results */
        emit infoASN(asModel);
    }

    end(reply);
}

void Bgpview::replyFinishedInfoAsnPrefixes(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(QUERY_TYPE == ASN_PREFIXES){
        AsModelStruct asModel;

        foreach(const QJsonValue &value, data["ipv4_prefixes"].toArray())
            asModel.prefixes.insert(value.toObject()["prefix"].toString());

        foreach(const QJsonValue &value, data["ipv6_prefixes"].toArray())
            asModel.prefixes.insert(value.toObject()["prefix"].toString());

        /* sending results */
        emit infoASN(asModel);
    }

    end(reply);
}

void Bgpview::replyFinishedInfoCidr(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QUERY_TYPE = reply->property(REQUEST_TYPE).toInt();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject data = document.object()["data"].toObject();

    if(QUERY_TYPE == IP_PREFIXES)
    {
        CidrModelStruct cidrModel;

        /* general info */
        cidrModel.info_prefix = data["prefix"].toString();
        cidrModel.info_ip = data["ip"].toString();
        cidrModel.info_cidr = QString::number(data["cidr"].toInt());
        cidrModel.info_name = data["name"].toString();
        cidrModel.info_description = data["description_short"].toString();
        cidrModel.info_country = data["country_code"].toString();
        cidrModel.info_website = data["website"].toString();
        // owner's address...
        QString address = "";
        foreach(const QJsonValue &value, data["owner_address"].toArray())
            address.append(value.toString()+", ");
        cidrModel.info_ownerAddress = address;

        /* email contacts */
        foreach(const QJsonValue &value, data["email_contacts"].toArray())
            cidrModel.emailcontacts.insert(value.toString());

        /* abuse contacts */
        foreach(const QJsonValue &value, data["abuse_contacts"].toArray())
            cidrModel.abusecontacts.insert(value.toString());

        /* rir allocation */
        QJsonObject rirAllocation = data["rir_allocation"].toObject();
        cidrModel.rir_name = rirAllocation["rir_name"].toString();
        cidrModel.rir_country = rirAllocation["country_code"].toString();
        cidrModel.rir_prefix = rirAllocation["prefix"].toString();
        cidrModel.rir_dateallocated = rirAllocation["date_allocated"].toString();

        /* asns */
        foreach(const QJsonValue &value, data["asns"].toArray()){
            QString asn = QString::number(value.toObject()["asn"].toInt());
            cidrModel.asns.insert(asn);
        }

        /* sending results */
        emit infoCidr(cidrModel);
    }

    end(reply);
}
