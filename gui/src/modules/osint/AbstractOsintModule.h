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
    ///
    /// for info...
    ///
    bool info = false;
    IpModel *ipModel = nullptr;
    ///
    /// for raw...
    ///
    int rawOption;
    bool raw = false;
    //...
    bool inputIp = false;
    bool inputAsn = false;
    bool inputCidr = false;
    bool inputEmail = false;
    bool inputDomain = false;
    bool inputCertFingerprint = false;
    //...
    bool outputCertFingerprint = false;
    bool outputSubdomainIp = false;
    bool outputSubdomain = false;
    bool outputEmail = false;
    bool outputAsn = false;
    bool outputUrl = false;
    bool outputIp = false;
    bool outputPrefixes = false;

    /* old */
    bool ip = false;
    bool urls = false;
    bool emails = false;
    bool subdomains = false;
    bool subdomainsAndIp = false;
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
        void certFingerprint(QString certId); // change to sslCert
        void ipCdir(QString ip);
        void ip(QString ip);
        void subdomain(QString subdomain);
        void subdomainIp(QString subdomain, QString ip);
        void email(QString email);
        void url(QString url);
        void asn(QString asn, QString asnName);
        void rawResults(QByteArray reply);
        void prefix(QString prefix, QString name);
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

    public slots:
        virtual void start() = 0;
        virtual void replyFinished(QNetworkReply*){}
        virtual void replyFinishedInfo(QNetworkReply*){}
        virtual void replyFinishedPrefixes(QNetworkReply *){} // ip/cidr prefixes
        virtual void replyFinishedCertFingerprint(QNetworkReply*){} // returns SSL Cert Sha1 fingerprint
        virtual void replyFinishedSubdomainIp(QNetworkReply*){} // returns subdomain and ip
        virtual void replyFinishedSubdomain(QNetworkReply*){} // returns subdomains
        virtual void replyFinishedIp(QNetworkReply*){} // returns ip-addresses
        virtual void replyFinishedAsn(QNetworkReply*){} // returns ASN
        virtual void replyFinishedEmail(QNetworkReply*){} // returns Emails
        virtual void replyFinishedUrl(QNetworkReply*){} // returns URLs
        virtual void replyFinishedRaw(QNetworkReply *reply) // returns raw results
        {
            if(reply->error())
                this->onError(reply);
            else
                emit rawResults(reply->readAll());

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
