#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QObject>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class AbstractModule : public QObject
{
        Q_OBJECT
    public:
        explicit AbstractModule(QObject *parent = nullptr) : QObject(parent){}
        ///
        /// the worker method that initiate scan...
        ///
        void Enumerator(QThread* cThread) const
        {
            connect(cThread, &QThread::started, this, &AbstractModule::start);
            connect(this, &AbstractModule::quitThread, cThread, &QThread::quit);
        }

    signals:
        void quitThread();
        void scanError(QString error);
        void scanResults(QString subdomain);

    public slots:
        virtual void start() = 0;
        virtual void  replyFinished(QNetworkReply *reply) = 0;

    public:
        QString target = nullptr;
        QNetworkAccessManager *manager = nullptr;
};

#endif // ABSTRACTMODULE_H
