#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H

#include <QObject>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

struct OsintScanStat{
    QString moduleName;
    unsigned int resultsCount;
};

struct ScanArgs{
    QString target;
    QString module;
    QString option;

    bool subdomains = false;
    bool subdomainsAndIp = false;
    bool ip = false;
    bool emails = false;
    bool urls = false;
    bool raw = false;
};

struct meta{
    QString moduleName;
    QString summary;
    QStringList useCases;
    QStringList flags;
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
        OsintScanStat *scanStat;
        ScanArgs *args;

    signals:
        void quitThread();
        void infoLog(QString log);
        void errorLog(QString error);
        void scanResults(QString subdomain);
        //...
        void ip(QString ip);
        void subdomains(QString subdomain);
        void subdomainIp(QString subdomain, QString ip);
        void emails(QString email);
        void urls(QString url);
        void rawResults(QByteArray reply);

    public slots:
        virtual void start() = 0;
        virtual void  replyFinished(QNetworkReply *reply) = 0;

    public:
        int maxPages = 100;
        QNetworkAccessManager *manager = nullptr;
};

#endif // ABSTRACTOSINTMODULE_H
