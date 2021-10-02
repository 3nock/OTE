#ifndef ABSTRACTOSINTMODULE_H
#define ABSTRACTOSINTMODULE_H

#include <QObject>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class AbstractOsintModule : public QObject{
        Q_OBJECT
    public:
        explicit AbstractOsintModule(QObject *parent = nullptr) : QObject(parent){}
        ///
        /// the worker method that initiates the scan...
        ///
        void Enumerator(QThread* cThread) const
        {
            connect(cThread, &QThread::started, this, &AbstractOsintModule::start);
            connect(this, &AbstractOsintModule::quitThread, cThread, &QThread::quit);
        }

    signals:
        void quitThread();
        void infoLog(QString log);
        void errorLog(QString error);
        void scanResults(QString subdomain);

    public slots:
        virtual void start() = 0;
        virtual void  replyFinished(QNetworkReply *reply) = 0;

    public:
        int maxPages = 100;
        QString target = nullptr;
        QNetworkAccessManager *manager = nullptr;
};

#endif // ABSTRACTOSINTMODULE_H
