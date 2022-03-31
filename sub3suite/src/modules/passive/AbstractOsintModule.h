#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "src/utils/s3s.h"
#include "src/utils/utils.h"
#include "OsintDefinitions.h"
#include "gumbo-parser/src/gumbo.h"

#include "src/items/IPItem.h"
#include "src/items/ASNItem.h"
#include "src/items/CIDRItem.h"
#include "src/items/SSLItem.h"
#include "src/items/EmailItem.h"
#include "src/items/MXItem.h"
#include "src/items/NSItem.h"
#include "src/items/RawItem.h"

/* input option */
#define IN_DOMAIN 0
#define IN_IP 1
#define IN_EMAIL 2
#define IN_URL 3
#define IN_ASN 4
#define IN_SSLCERT 5
#define IN_CIDR 6
#define IN_QUERYTERM 7

/* output option */
#define OUT_SUBDOMAIN 0
#define OUT_SUBDOMAINIP 1
#define OUT_IP 2
#define OUT_EMAIL 3
#define OUT_URL 4
#define OUT_ASN 5
#define OUT_SSLCERT 6
#define OUT_CIDR 7


struct ScanLog {
    QString moduleName;
    QString message;
    QString target;
    int statusCode = 0;
    uint resultsCount = 0;
    bool error = false;
};

struct ScanConfig {
    int maxPage = 50;
    bool setTimeout = false;
    bool noDuplicates = false;
    bool autosaveToProject = false;
    int timeout = 2000;
    int progress = 0;
};

struct ScanArgs {
    ScanConfig *config;
    QQueue<QString> targets;

    /* input type */
    bool input_IP = false;
    bool input_ASN = false;
    bool input_CIDR = false;
    bool input_URL = false;
    bool input_SSL = false;
    bool input_Email = false;
    bool input_Domain = false;
    bool input_Search = false;

    /* output type */
    bool output_HostnameIP = false;
    bool output_Hostname = false;
    bool output_Email = false;
    bool output_ASN = false;
    bool output_URL = false;
    bool output_IP = false;
    bool output_CIDR = false;
    bool output_SSL = false;
    bool output_Raw = false;

    bool output_EnumIP = false;
    bool output_EnumCIDR = false;
    bool output_EnumSSL = false;
    bool output_EnumMX = false;
    bool output_EnumNS = false;
    bool output_EnumEmail = false;
    bool output_EnumASN = false;
    bool output_EnumASNPeers = false;
    bool output_EnumASNPrefixes = false;

    /* for raw output */
    int raw_query_id = 0;
    QString raw_query_name;
};

///
/// getting html body node...
///

GumboNode* getBody(GumboNode *node) {
    for(unsigned int i = 0; i < node->v.element.children.length; i++)
    {
        GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[i]);
        if(child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_BODY)
            return child;
    }
    return nullptr;
}

///
/// \brief The AbstractOsintModule class
///

class AbstractOsintModule : public QObject {
    Q_OBJECT

public:
    explicit AbstractOsintModule(ScanArgs args)
        : QObject(nullptr),
          args(args)
    {
    }
    ~AbstractOsintModule()
    {
    }

    void startScan(QThread* cThread)
    {
        connect(cThread, &QThread::started, this, &AbstractOsintModule::start);
        connect(this, &AbstractOsintModule::nextTarget, this, &AbstractOsintModule::start);
        connect(this, &AbstractOsintModule::quitThread, cThread, &QThread::quit);

        /* first target */
        target = args.targets.dequeue();
        log.target = target;
    }

signals:
    void quitThread();
    void nextTarget();

    void scanProgress(int progress);
    void scanLog(ScanLog);

    void resultSubdomain(QString subdomain);
    void resultSubdomainIp(QString subdomain, QString ip);
    void resultIP(QString ip);
    void resultA(QString ip);
    void resultAAAA(QString ip);
    void resultNS(QString NS);
    void resultMX(QString MX);
    void resultCNAME(QString CNAME);
    void resultTXT(QString TXT);
    void resultSSL(QString certId);
    void resultEmail(QString email);
    void resultCIDR(QString cidr);
    void resultURL(QString url);
    void resultASN(QString asn, QString name);

    void resultRawSSL(QByteArray);
    void resultRawTXT(s3s_struct::RAW);
    void resultRawJSON(s3s_struct::RAW);

    void resultEnumASN(s3s_struct::ASN);
    void resultEnumCIDR(s3s_struct::CIDR);
    void resultEnumMX(s3s_struct::MX);
    void resultEnumNS(s3s_struct::NS);
    void resultEnumIP(s3s_struct::IP);
    void resultEnumEmail(s3s_struct::Email);

public slots:
    void onStop(){
        log.statusCode = 0;
        log.message = "Stopped...";
        emit scanLog(log);
        emit quitThread();
    }

protected slots:
    virtual void start() = 0;
    virtual void replyFinishedSubdomainIp(QNetworkReply*){} // returns subdomain and ip
    virtual void replyFinishedSubdomain(QNetworkReply*){} // returns subdomains
    virtual void replyFinishedCidr(QNetworkReply *){} // returns ip/cidr
    virtual void replyFinishedSSL(QNetworkReply*){} // returns SSL Cert Sha1 fingerprint
    virtual void replyFinishedIp(QNetworkReply*){} // returns ip-addresses
    virtual void replyFinishedAsn(QNetworkReply*){} // returns ASN
    virtual void replyFinishedEmail(QNetworkReply*){} // returns Emails
    virtual void replyFinishedUrl(QNetworkReply*){} // returns URLs

    virtual void replyFinishedEnumASN(QNetworkReply*){} // returns multiple info on asn
    virtual void replyFinishedEnumASNPeers(QNetworkReply*){} // returns multiple info on asn peers
    virtual void replyFinishedEnumASNPrefixes(QNetworkReply*){} // returns multiple info on asn prefixes

    virtual void replyFinishedEnumIP(QNetworkReply*){} // returns multiple info on ip
    virtual void replyFinishedEnumCIDR(QNetworkReply*){} // returns multiple info on cidr
    virtual void replyFinishedEnumSSL(QNetworkReply*){} // returns multiple info on ssl cert

    virtual void replyFinishedEnumMX(QNetworkReply*){} // returns MX records info
    virtual void replyFinishedEnumNS(QNetworkReply*){} // returns NS records info

    virtual void replyFinishedEnumEmail(QNetworkReply*){} // returns Email info

    virtual void replyFinishedRawNdjson(QNetworkReply *reply) // returns raw json results from ndjson
    {
        if(reply->error())
            this->onError(reply);
        else
        {
            /* converting ndjson to json array document */
            QByteArray byteDocument = reply->readAll();
            byteDocument = byteDocument.simplified();
            byteDocument.replace("\n", ",");
            byteDocument.push_back("]");
            byteDocument.push_front("[");

            s3s_struct::RAW raw;
            raw.module = log.moduleName;
            raw.query_option = args.raw_query_name;
            raw.target = target;
            raw.results = byteDocument;
            emit resultRawJSON(raw);
        }

        this->end(reply);
    }

    virtual void replyFinishedRawJson(QNetworkReply *reply) // returns raw json results
    {
        if(reply->error())
            this->onError(reply);
        else{
            s3s_struct::RAW raw;
            raw.module = log.moduleName;
            raw.query_option = args.raw_query_name;
            raw.target = target;
            raw.results = reply->readAll();
            emit resultRawJSON(raw);
        }

        this->end(reply);
    }

    virtual void replyFinishedRawTxt(QNetworkReply *reply) // returns raw txt results
    {
        if(reply->error())
            this->onError(reply);
        else{
            s3s_struct::RAW raw;
            raw.module = log.moduleName;
            raw.query_option = args.raw_query_name;
            raw.target = target;
            raw.results = reply->readAll();
            emit resultRawTXT(raw);
        }

        this->end(reply);
    }

protected:
    ScanLog log;
    ScanArgs args;
    QString target;
    s3sNetworkAccessManager *manager = nullptr;

    void checkAPIKey(QString key){
        if(key.isNull() || key.isEmpty()){
            log.message = "API key Required!";
            log.statusCode = 0;
            log.error = true;
            emit scanLog(log);
            emit quitThread();
        }
    }

    void onError(QNetworkReply *reply) {
        switch(reply->error()){
        case QNetworkReply::OperationCanceledError:
            log.target = target;
            log.message = "Operation Cancelled due to Timeout";
            log.statusCode = 0;
            log.error = true;
            emit scanLog(log);
            break;
        default:
            log.target = target;
            log.message = reply->errorString();
            log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            log.error = true;
            emit scanLog(log);
            break;
        }

        reply->close();
        reply->deleteLater();
        this->next();
    }

    void end(QNetworkReply *reply) {
        log.target = target;
        log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        emit scanLog(log);

        reply->close();
        reply->deleteLater();
        this->next();
    }

    void next() {
        args.config->progress++;
        emit scanProgress(args.config->progress);

        /*
         * enumerate next target if there are still targets available
         * if no targets available quit the scanThread.
         */
        if(args.targets.length()){
            target = args.targets.dequeue();
            emit nextTarget();
        }
        else
            emit quitThread();
    }
};

#endif // ABSTRACTOSINTMODULE_H
