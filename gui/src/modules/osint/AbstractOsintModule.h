#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H

#include <QObject>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

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
    /// for raw...
    ///
    int rawOption;
    bool raw = false;
    //...
    bool inputIp = false;
    bool inputAsn = false;
    bool inputEmail = false;
    bool inputDomain = false;
    //...
    bool outputSubdomainIp = false;
    bool outputSubdomain = false;
    bool outputEmail = false;
    bool outputAsn = false;
    bool outputUrl = false;
    bool outputIp = false;

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
        void ip(QString ip);
        void subdomain(QString subdomain);
        void subdomainIp(QString subdomain, QString ip);
        void email(QString email);
        void url(QString url);
        void asn(QString asn, QString asnName);
        void rawResults(QByteArray reply);
        /* dns */
        void ipA(QString ip);
        void ipAAAA(QString ip);
        void NS(QString NS);
        void MX(QString MX);
        /* old */
        void errorLog(QString log);
        void infoLog(QString log);

    public slots:
        virtual void start() = 0;
        virtual void replyFinished(QNetworkReply*){}
        virtual void replyFinishedSubdomainIp(QNetworkReply*){} // returns subdomain and ip
        virtual void replyFinishedSubdomain(QNetworkReply*){} // returns subdomains
        virtual void replyFinishedIp(QNetworkReply*){} // returns ip-addresses
        virtual void replyFinishedAsn(QNetworkReply*){} // returns ASN
        virtual void replyFinishedEmail(QNetworkReply*){} // returns Emails
        virtual void replyFinishedUrl(QNetworkReply*){} // returns URLs
        virtual void replyFinishedRaw(QNetworkReply *reply) // returns raw results...
        {
            if(reply->error())
                this->onError(reply);
            else
                emit rawResults(reply->readAll());

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
