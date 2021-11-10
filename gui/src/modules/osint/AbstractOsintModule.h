#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H

#include <QObject>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
//...
#include "src/models/IpModel.h"
#include "src/models/AsnModel.h"

#define REQUEST_TYPE "type"
///
/// input types for raw placeholder txt...
///
#define PLACEHOLDERTEXT_DOMAIN "domain name eg example.com"
#define PLACEHOLDERTEXT_IP "ip-address eg 8.8.8.8"
#define PLACEHOLDERTEXT_IP4 "IpV4-address eg 8.8.8.8"
#define PLACEHOLDERTEXT_IP_OR_CIDR "ip-address eg 1.1.1.1 or ip-address/cidr eg 1.1.1.1/24"
#define PLACEHOLDERTEXT_DOMAIN_OR_IP "domain name eg example.com or ip-address eg 1.1.1.1"
#define PLACEHOLDERTEXT_IP6 "IpV6-address eg 2001:4860:4860::8888"
#define PLACEHOLDERTEXT_ASN "asn number eg 12345"
#define PLACEHOLDERTEXT_EMAIL "email-address eg something@email.com"
#define PLACEHOLDERTEXT_CIDR "ip-address/cidr eg 1.1.1.1/24"
#define PLACEHOLDERTEXT_QUERY "query term eg GOOGLE"
#define PLACEHOLDERTEXT_SSLCERT "ssl cert sha1 eg "
#define PLACEHOLDERTEXT_PHONE "phone number eg +2557298984323"
#define PLACEHOLDERTEXT_USERNAME "username eg John Doe"
#define PLACEHOLDERTEXT_URL "url eg https://example.com/something"
#define PLACEHOLDERTEXT_NONE ""
#define PLACEHOLDERTEXT_NS "dns server name eg dns.google.com"
#define PLACEHOLDERTEXT_HOSTNAME "hostname eg test.example.com"
#define PLACEHOLDERTEXT_ORG "organization name eg GOOGLE LLC"
#define PLACEHOLDERTEXT_MX "mailserver name eg aspmx.l.google.com"
#define PLACEHOLDERTEXT_MAC_ADDRESS "mac address eg 00-05-02-34-56-78"
#define PLACEHOLDERTEXT_POC "point of contact eg KOSTE-ARIN"
#define PLACEHOLDERTEXT_NET "network eg"
#define PLACEHOLDERTEXT_CHECKSUM "sha256 checksum eg"
#define PLACEHOLDERTEXT_MD5 "md5 eg"
#define PLACEHOLDERTEXT_IP_OR_ASN_OR_CIDR "ip or asn or cidr"
#define PLACEHOLDERTEXT_IP_OR_ASN "ip or asn"
#define PLACEHOLDERTEXT_IP_OR_DOMAIN "ip or domain"
#define PLACEHOLDERTEXT_IP_RANGE "ip range"
#define PLACEHOLDERTEXT_IP_RANGE_OR_CIDR "ip range or cidr"
#define PLACEHOLDERTEXT_AS "as eg AS15169"
#define PLACEHOLDERTEXT_COUNTRY_CODE "2-digit ISO-3166 country code e.g. tz"
#define PLACEHOLDERTEXT_ANY "asn, ip, cidr, organization"
#define PLACEHOLDERTEXT_CIDR_OR_ASN "cidr or asn"

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

    protected:
        int activeRequests = 0;
        ScanArgs *args;
        ScanLog log;
        int maxPages = 100;
        MyNetworkAccessManager *manager = nullptr;
};

#endif // ABSTRACTOSINTMODULE_H
