#ifndef IPSCANNER_H
#define IPSCANNER_H

#include "AbstractScanner.h"

#include <QQueue>
#include <QMutex>
#include <QHostInfo>

namespace ip {

struct ScanStat {  // scan statistics
    int nameservers = 0;
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    QQueue<QString> nameservers;
    int threads = 50;
    int timeout = 3000;

    bool setTimeout = false;
    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    ip::ScanConfig *config;
    QQueue<QString> targets;
    QHostAddress nameserver;
    int progress;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        explicit Scanner(ip::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished(QHostInfo);

    signals:
        void next(); // next lookup
        void scanResult(QString ip, QString hostname); // send active enumerated results

    private:
        ip::ScanArgs *m_args;
};

QString getTarget(ip::ScanArgs *args);

}

#endif // IPSCANNER_H
