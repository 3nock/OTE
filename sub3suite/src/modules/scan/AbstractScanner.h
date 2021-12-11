#ifndef ABSTRACTSCANNER_H
#define ABSTRACTSCANNER_H

#include <QObject>
#include <QThread>
#include "src/utils/utils.h"

/*
 * count and keep log of subdomainIp the Error lookups...
 */

struct ScanStats{
    unsigned int successfulLookups;
    unsigned int errorLookups;
};

class AbstractScanner : public QObject{
    Q_OBJECT

    public:
        AbstractScanner(QObject *parent = nullptr)
            : QObject(parent)
        {
        }
        ///
        /// start the scan Object...
        ///
        void startScan(QThread *cThread)
        {
            connect(cThread, &QThread::started, this, &AbstractScanner::lookup);
            connect(this, &AbstractScanner::quitThread, cThread, &QThread::quit);
        }

    protected:
        ScanStats *scanStats;

    public slots:
        ///
        /// the main method that performs the lookup...
        ///
        virtual void lookup() = 0;
        ///
        /// quiting subdomainIp running threads upon receiving stop signal...
        ///
        void onStopScan(){
            emit quitThread();
        }

    signals:
        void quitThread();
        void infoLog(QString log);
        void errorLog(QString log);
        void scanProgress(int value);
        ///
        /// for certScanner...
        ///
        void resultSHA1(QString);
        void resultSHA256(QString);
        void resultRaw(QByteArray);
        void resultSubdomain(QString);
        ///
        /// for banner...
        ///
        void scanResultBanner(QString banner);
};

#endif // ABSTRACTSCANNER_H
