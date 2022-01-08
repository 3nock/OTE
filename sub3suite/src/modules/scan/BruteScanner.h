#ifndef BRUTESCANNER_H
#define BRUTESCANNER_H

#include <QMutex>
#include <QQueue>
#include <QDnsLookup>
#include <QHostAddress>
#include "AbstractScanner.h"


namespace brute {

struct ScanLog{
    bool error = false;
    bool info = false;

    QString target;
    QString message;
    QString nameserver;
};

struct ScanConfig{
    QDnsLookup::Type recordType = QDnsLookup::A;
    QStringList nameservers;
    int levels = 0;
    int threads = 50;
    int timeout = 3000;

    bool noDuplicates = false;
    bool autoSaveToProject = false;
    bool multiLevelScan = false;
    bool checkWildcard = false;
};

struct ScanArgs {
    brute::ScanConfig *config;
    QQueue<QString> targets;
    QStringList wordlist;
    QString currentTarget;
    int currentWordlist;
    int progress;
    bool subdomain;
    bool tld;
};

class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(brute::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished();

    signals:
        void next(); // next lookup
        void scanLog(brute::ScanLog log); // scan error log
        void scanResult(QString subdomain, QString ip); // lookup results

    private:
        brute::ScanArgs *m_args;
        QDnsLookup *m_dns;
};

RetVal lookupSubdomain(QDnsLookup*, ScanArgs*);
RetVal lookupTLD(QDnsLookup*, ScanArgs*);

}
#endif //BRUTE_H
