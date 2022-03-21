#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QBasicTimer>
#include <QTimerEvent>

#include "OsintDefinitions.h"
#include "src/utils/s3s.h"
#include "src/utils/utils.h"
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


struct ScanLog{
    QString moduleName;
    QString message;
    QString target;
    int statusCode = 0;
    unsigned int resultsCount = 0;
};

struct ScanConfig{
    int maxPage = 50;
    bool noDuplicates = false;
    bool autosaveToProject = false;
    int timeout = 2000;
    int progress = 0;
};

struct ScanArgs{
    ScanConfig *config;
    QQueue<QString> targets;

    /* input type */
    bool inputIp = false;
    bool inputAsn = false;
    bool inputCidr = false;
    bool inputUrl = false;
    bool inputEmail = false;
    bool inputDomain = false;
    bool inputSSL = false;
    bool inputQueryTerm = false;

    /* output type */
    bool outputSubdomainIp = false;
    bool outputSubdomain = false;
    bool outputEmail = false;
    bool outputAsn = false;
    bool outputUrl = false;
    bool outputIp = false;
    bool outputCidr = false;
    bool outputSSL = false;
    bool outputRaw = false;

    bool outputInfoIp = false;
    bool outputInfoCidr = false;
    bool outputInfoSSL = false;
    bool outputInfoMX = false;
    bool outputInfoNS = false;
    bool outputInfoEmail = false;
    bool outputInfoAsn = false;
    bool outputInfoAsnPeers = false;
    bool outputInfoAsnPrefixes = false;


    /* for raw output */
    int rawOption = 0;
    QString queryOption;
    QMap<int, QString> rawParameters;
};


class AbstractOsintModule : public QObject {
        Q_OBJECT

    public:
        explicit AbstractOsintModule(ScanArgs args)
            : QObject(nullptr),
              args(args)
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

        void infoLog(ScanLog log);
        void errorLog(ScanLog error);

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

        void rawSSL(QByteArray);
        void rawResults(s3s_struct::RAW);
        void rawResultsTxt(s3s_struct::RAW);

        void infoASN(s3s_struct::ASN);
        void infoCIDR(s3s_struct::CIDR);
        void infoMX(s3s_struct::MX);
        void infoNS(s3s_struct::NS);
        void infoIp(s3s_struct::IP);
        void infoEmail(s3s_struct::Email);

    public slots:
        void onStop(){
            log.statusCode = 0;
            log.message = "Stopped...";
            emit infoLog(log);
            emit quitThread();
        }

    protected slots:
        virtual void start() = 0;
        virtual void replyFinishedSubdomain(QNetworkReply*){} // returns subdomains
        virtual void replyFinishedCidr(QNetworkReply *){} // returns ip/cidr
        virtual void replyFinishedSSL(QNetworkReply*){} // returns SSL Cert Sha1 fingerprint
        virtual void replyFinishedSubdomainIp(QNetworkReply*){} // returns subdomain and ip
        virtual void replyFinishedIp(QNetworkReply*){} // returns ip-addresses
        virtual void replyFinishedAsn(QNetworkReply*){} // returns ASN
        virtual void replyFinishedEmail(QNetworkReply*){} // returns Emails
        virtual void replyFinishedUrl(QNetworkReply*){} // returns URLs

        virtual void replyFinishedInfoAsn(QNetworkReply*){} // returns multiple info on asn
        virtual void replyFinishedInfoAsnPeers(QNetworkReply*){} // returns multiple info on asn peers
        virtual void replyFinishedInfoAsnPrefixes(QNetworkReply*){} // returns multiple info on asn prefixes

        virtual void replyFinishedInfo(QNetworkReply*){} // returns multiple info on appropriate target
        virtual void replyFinishedInfoIp(QNetworkReply*){} // returns multiple info on ip
        virtual void replyFinishedInfoCidr(QNetworkReply*){} // returns multiple info on cidr
        virtual void replyFinishedInfoSSL(QNetworkReply*){} // returns multiple info on ssl cert

        virtual void replyFinishedInfoMX(QNetworkReply*){} // returns MX records info
        virtual void replyFinishedInfoNS(QNetworkReply*){} // returns NS records info

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
                raw.query_option = args.queryOption;
                raw.target = target;
                raw.results = byteDocument;
                emit rawResults(raw);
            }

            end(reply);
        }

        virtual void replyFinishedRawJson(QNetworkReply *reply) // returns raw json results
        {
            if(reply->error())
                this->onError(reply);
            else{
                s3s_struct::RAW raw;
                raw.module = log.moduleName;
                raw.query_option = args.queryOption;
                raw.target = target;
                raw.results = reply->readAll();
                emit rawResults(raw);
            }

            end(reply);
        }

        virtual void replyFinishedRawTxt(QNetworkReply *reply) // returns raw txt results
        {
            if(reply->error())
                this->onError(reply);
            else{
                s3s_struct::RAW raw;
                raw.module = log.moduleName;
                raw.query_option = args.queryOption;
                raw.target = target;
                raw.results = reply->readAll();
                emit rawResultsTxt(raw);
            }

            end(reply);
        }

    protected:
        ScanLog log;
        ScanArgs args;
        QString target;
        int activeRequests = 0;
        int QUERY_TYPE;
        s3sNetworkAccessManager *manager = nullptr;

        void onError(QNetworkReply *reply){
            switch(reply->error()){
            case QNetworkReply::OperationCanceledError:
                log.target = target;
                log.message = "Operation Cancelled due to Timeout";
                log.statusCode = 0;
                emit errorLog(log);
                break;
            default:
                log.target = target;
                log.message = reply->errorString();
                log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                emit errorLog(log);
                break;
            }

            /* has its own end */
            reply->close();
            reply->deleteLater();
            activeRequests--;
            if(activeRequests == 0)
                emit quitThread();
        }

        void end(QNetworkReply *reply){
            log.target = target;
            log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            reply->close();
            reply->deleteLater();
            activeRequests--;
            if(activeRequests == 0)
            {
                /* send logs on the target scanned */
                emit infoLog(log);

                /* scan prohress */
                args.config->progress++;
                emit scanProgress(args.config->progress);

                /* enumerate next target if there are still targets available */
                if(args.targets.length()){
                    target = args.targets.dequeue();
                    emit nextTarget();
                }

                /* if no targets available quit the scanThread */
                else
                    emit quitThread();
            }
        }

        /* make this a normal function not a class object */
        GumboNode* getBody(GumboNode *node){
            for(unsigned int i = 0; i < node->v.element.children.length; i++)
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[i]);
                if(child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_BODY)
                    return child;
            }
            return nullptr;
        }
};

#endif // ABSTRACTOSINTMODULE_H
