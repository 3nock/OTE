#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H


#include <QObject>
#include <QThread>
#include <QStack>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
/* ... */
#include "OsintDefinitions.h"
#include "src/utils/Definitions.h"
#include "gumbo-parser/src/gumbo.h"
/* ... */
#include "src/models/IpModel.h"
#include "src/models/AsnModel.h"
#include "src/models/CidrModel.h"
#include "src/models/CertModel.h"
#include "src/models/DomainModel.h"
#include "src/models/EmailModel.h"
#include "src/models/MXModel.h"
#include "src/models/NSmodel.h"


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

/* ... */
#define REQUEST_TYPE "type"


/* stores the scan configurations */
struct PassiveScanConfig{
    bool useProxy = false;
    QString proxyAddress;
    QString proxyPort;
    /* ... */
    int timeout;
    int maxPage;
};


/*
 * all arguments that an osint module needs including
 * scan configurations
 */
struct ScanArgs{
    /* scan configurations */
    PassiveScanConfig *config;

    /* target */
    QStack<QString> targets;

    /* input type */
    bool inputIp = false;
    bool inputAsn = false;
    bool inputCidr = false;
    bool inputUrl = false;
    bool inputEmail = false;
    bool inputDomain = false;
    bool inputSSLCert = false;

    /* output type */
    bool outputRaw = false;
    /* ... */
    bool outputInfo = false;
    bool outputInfoIp = false;
    bool outputInfoCidr = false;
    bool outputInfoSSLCert = false;
    /* ... */
    bool outputInfoAsn = false;
    bool outputInfoAsnPeers = false;
    bool outputInfoAsnPrefixes = false;
    /* ... */
    bool outputSubdomainIp = false;
    bool outputSubdomain = false;
    bool outputEmail = false;
    bool outputAsn = false;
    bool outputUrl = false;
    bool outputIp = false;
    bool outputCidr = false;
    bool outputSSLCert = false;

    /* for raw output */
    int rawOption = 0;
    QMap<int, QString> rawParameters;
};

struct ScanLog{
    QString moduleName;
    QString message;
    QString target;
    int statusCode;
    unsigned int resultsCount;
};


class NetworkAccessManager: public QNetworkAccessManager {
    public:
        NetworkAccessManager(QObject *parent = nullptr): QNetworkAccessManager(parent)
        {
        }

    protected:
        QNetworkReply* createRequest(Operation op, const QNetworkRequest &request, QIODevice *data = nullptr)
        {
            QNetworkReply *reply = QNetworkAccessManager::createRequest(op, request, data);
            reply->setProperty(REQUEST_TYPE, request.attribute(QNetworkRequest::User));
            return reply;
        }
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
            /* signal & slots */
            connect(cThread, &QThread::started, this, &AbstractOsintModule::start); // to start the enumeration when the thread starts
            connect(this, &AbstractOsintModule::nextTarget, this, &AbstractOsintModule::start); // goes to next target
            connect(this, &AbstractOsintModule::quitThread, cThread, &QThread::quit); // ends(terminates) the thread

            /* first target */
            target = args.targets.pop();
            log.target = target;
        }

    signals:
        void quitThread();
        void nextTarget();
        /* ... */
        void infoLog(ScanLog log);
        void rateLimitLog(ScanLog log);
        void errorLog(ScanLog error);
        /* ... */
        void subdomain(QString subdomain);
        void subdomainIp(QString subdomain, QString ip);
        void ip(QString ip);
        void ipA(QString ip);
        void ipAAAA(QString ip);
        void NS(QString NS);
        void MX(QString MX);
        void CNAME(QString CNAME);
        void TXT(QString TXT);
        void sslCert(QString certId);
        void email(QString email);
        void cidr(QString cidr);
        void url(QString url);
        void asn(QString asn, QString name);
        /* ... */
        void rawCert(QByteArray cert_in_perm_format);
        void rawResults(QByteArray reply);
        void rawResultsTxt(QByteArray reply);
        /* ... */
        void infoASN(AsModelStruct);
        void infoCidr(CidrModelStruct);

    public slots:
        void onStop(){
            log.statusCode = 0;
            log.message = "Stopped...";
            emit infoLog(log);
            emit quitThread();
        }

        void onPause(){
            log.statusCode = 0;
            log.message = "Paused...";
            emit infoLog(log);
            emit quitThread();
        }

    protected slots:
        virtual void start() = 0;
        virtual void replyFinishedSubdomain(QNetworkReply*){} // returns subdomains
        virtual void replyFinishedCidr(QNetworkReply *){} // returns ip/cidr
        virtual void replyFinishedSSLCert(QNetworkReply*){} // returns SSL Cert Sha1 fingerprint
        virtual void replyFinishedSubdomainIp(QNetworkReply*){} // returns subdomain and ip
        virtual void replyFinishedIp(QNetworkReply*){} // returns ip-addresses
        virtual void replyFinishedAsn(QNetworkReply*){} // returns ASN
        virtual void replyFinishedEmail(QNetworkReply*){} // returns Emails
        virtual void replyFinishedUrl(QNetworkReply*){} // returns URLs
        /* ... */
        virtual void replyFinishedInfoAsn(QNetworkReply*){} // returns multiple info on asn
        virtual void replyFinishedInfoAsnPeers(QNetworkReply*){} // returns multiple info on asn peers
        virtual void replyFinishedInfoAsnPrefixes(QNetworkReply*){} // returns multiple info on asn prefixes
        /* ... */
        virtual void replyFinishedInfo(QNetworkReply*){} // returns multiple info on appropriate target
        virtual void replyFinishedInfoIp(QNetworkReply*){} // returns multiple info on ip
        virtual void replyFinishedInfoCidr(QNetworkReply*){} // returns multiple info on cidr
        virtual void replyFinishedInfoSSLCert(QNetworkReply*){} // returns multiple info on ssl cert
        /* ... */
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

                emit rawResults(byteDocument);
            }

            end(reply);
        }

        virtual void replyFinishedRawJson(QNetworkReply *reply) // returns raw json results
        {
            if(reply->error())
                this->onError(reply);
            else
                emit rawResults(reply->readAll());

            end(reply);
        }

        virtual void replyFinishedRawTxt(QNetworkReply *reply) // returns raw txt results
        {
            if(reply->error())
                this->onError(reply);
            else
                emit rawResultsTxt(reply->readAll());

            end(reply);
        }

    protected:
        ScanLog log;
        ScanArgs args;
        QString target;
        int activeRequests = 0;
        int QUERY_TYPE;
        NetworkAccessManager *manager = nullptr;

        void  onRateLimit(QNetworkReply *reply){
            log.message = "API rate limit reached";
            log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            emit rateLimitLog(log);

            /* has its own end */
            reply->deleteLater();
            activeRequests--;
            if(activeRequests == 0)
                emit quitThread();
        }

        void onError(QNetworkReply *reply){
            log.message = reply->errorString();
            log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            emit errorLog(log);

            /* has its own end */
            reply->deleteLater();
            activeRequests--;
            if(activeRequests == 0)
                emit quitThread();
        }

        void end(QNetworkReply *reply){
            log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            reply->deleteLater();
            activeRequests--;
            if(activeRequests == 0)
            {
                /* send logs on the target scanned */
                emit infoLog(log);

                /* enumerate next target if there are still targets available */
                if(args.targets.length()){
                    target = args.targets.pop();
                    emit nextTarget();
                }

                /* if no targets available quit the scanThread */
                else
                    emit quitThread();
            }
        }

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
