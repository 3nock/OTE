#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H

#include <QObject>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
//...
#include "src/utils/Definitions.h"
#include "gumbo-parser/src/gumbo.h"
//...
#include "src/models/IpModel.h"
#include "src/models/AsnModel.h"

#define REQUEST_TYPE "type"

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
#define OUT_SUBDOMAINIP 0
#define OUT_SUBDOMAIN 1
#define OUT_IP 2
#define OUT_EMAIL 3
#define OUT_URL 4
#define OUT_ASN 5
#define OUT_SSLCERT 6
#define OUT_CIDR 7

struct ScanLog{
    QString moduleName;
    QString message;
    int statusCode;
    unsigned int resultsCount;
};

struct ScanArgs{
    QString target;
    QString module;
    QString option;

    /* input type */
    bool inputIp = false;
    bool inputAsn = false;
    bool inputCidr = false;
    bool inputUrl = false;
    bool inputEmail = false;
    bool inputDomain = false;
    bool inputSSLCert = false;

    /* output type */
    bool outputSSLCert = false;
    bool outputSubdomainIp = false;
    bool outputSubdomain = false;
    bool outputEmail = false;
    bool outputAsn = false;
    bool outputUrl = false;
    bool outputIp = false;
    bool outputCidr = false;

    /* for info... */
    bool info = false;
    IpModel *ipModel = nullptr;

    /* for raw... */
    int rawOption;
    bool raw = false;
};

class MyNetworkAccessManager: public QNetworkAccessManager {
    public:
        MyNetworkAccessManager(QObject *parent = nullptr): QNetworkAccessManager(parent)
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

/*
 * create an error system...
 */

class AbstractOsintModule : public QObject {
        Q_OBJECT

    public:
        explicit AbstractOsintModule(ScanArgs *args)
            : QObject(nullptr),
              args(args)
        {
        }
        ///
        /// the worker method that initiates the scan...
        ///
        void Enumerator(QThread* cThread) const
        {
            connect(cThread, &QThread::started, this, &AbstractOsintModule::start);
            connect(this, &AbstractOsintModule::quitThread, cThread, &QThread::quit);
        }

    protected:
        void onError(QNetworkReply *reply){
            log.message = reply->errorString();
            //emit errorLog(log);
            emit errorLog(reply->errorString());
            end(reply); // for now, later implement own end...
        }

        inline void end(QNetworkReply *reply){
            log.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            reply->deleteLater();
            activeRequests--;
            if(activeRequests == 0){
                //emit errorLog(log);
                emit quitThread();
            }
        }

        GumboNode* getBody(GumboNode *node){
            for(unsigned int i = 0; i < node->v.element.children.length; i++){
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[i]);
                if(child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_BODY)
                    return child;
            }
            return nullptr;
        }

    signals:
        ///
        /// self signal...
        ///
        void quitThread();
        ///
        /// signals to Osint engine...
        ///
        /* void infoLog(ScanLog log);
           void errorLog(ScanLog error);
        */
        void sslCert(QString certId); // change to sslCert
        void cidr(QString cidr);
        void ip(QString ip);
        void subdomain(QString subdomain);
        void subdomainIp(QString subdomain, QString ip);
        void email(QString email);
        void url(QString url);
        void asn(QString asn, QString asnName);
        void rawResults(QByteArray reply);
        void rawResultsTxt(QByteArray reply);
        /* dns */
        void ipA(QString ip);
        void ipAAAA(QString ip);
        void NS(QString NS);
        void MX(QString MX);
        void CNAME(QString CNAME);
        void TXT(QString TXT);
        /* old */
        void errorLog(QString log);
        void infoLog(QString log);
        /* for ssl-cert info */
        void rawCert(QByteArray);

    public slots:
        virtual void start() = 0;
        virtual void replyFinishedSubdomain(QNetworkReply*){}
        virtual void replyFinishedInfo(QNetworkReply*){}
        virtual void replyFinishedCidr(QNetworkReply *){} // ip/cidr
        virtual void replyFinishedSSLCert(QNetworkReply*){} // returns SSL Cert Sha1 fingerprint
        virtual void replyFinishedSubdomainIp(QNetworkReply*){} // returns subdomain and ip
        virtual void replyFinishedIp(QNetworkReply*){} // returns ip-addresses
        virtual void replyFinishedAsn(QNetworkReply*){} // returns ASN
        virtual void replyFinishedEmail(QNetworkReply*){} // returns Emails
        virtual void replyFinishedUrl(QNetworkReply*){} // returns URLs
        virtual void replyFinishedRaw(QNetworkReply *reply) // returns raw json results
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
        virtual void replyFinishedRawNdjson(QNetworkReply *reply) // returns raw results from ndjson
        {
            if(reply->error())
                this->onError(reply);
            else{
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

    protected:
        int activeRequests = 0;
        ScanArgs *args;
        ScanLog log;
        int maxPages = 100;
        MyNetworkAccessManager *manager = nullptr;
};

#endif // ABSTRACTOSINTMODULE_H
