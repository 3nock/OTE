#ifndef IPSCANNER_H
#define IPSCANNER_H

#include "AbstractScanner.h"
#include "PortScanner.h"
#include "PingScanner.h"
#include <QQueue>
#include <QMutex>
#include <QHostInfo>

namespace reverseip {

struct ScanStat {  // scan statistics
    int nameservers = 0;
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    QQueue<QString> nameservers;
    port::ScanArgs *portScanConfig;
    ping::ScanArgs *pingScanConfig;
    int threads = 50;
    int timeout = 3000;

    bool setTimeout = false;
    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    reverseip::ScanConfig *config;
    QQueue<QString> targets;
    QHostAddress nameserver;
    int progress;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        explicit Scanner(reverseip::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;

    signals:
        void scanResult(QString ip, QString hostname); // send active enumerated results

    public slots:
        virtual void onStopScan() override {
            stop = true;
        }

        void onPauseScan() override {
            m_mutex.lock();
            pause = true;
            m_mutex.unlock();
        }

        void onResumeScan() override {
            m_mutex.lock();
            pause = false;
            m_mutex.unlock();
            m_wait.wakeAll();
        }

    private:
        reverseip::ScanArgs *m_args;
        QString m_target;
        bool stop = false;
        bool pause = false;
        QWaitCondition m_wait;
        QMutex m_mutex;
};

QString getTarget(reverseip::ScanArgs *args);

}

#endif // IPSCANNER_H
