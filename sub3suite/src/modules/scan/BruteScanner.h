#ifndef BRUTESCANNER_H
#define BRUTESCANNER_H

#include <QMutex>
#include <QQueue>
#include "AbstractScanner.h"


namespace brute {

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
};

class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(brute::ScanArgs *args);
        ~Scanner() override;

        void startScanSubdomain(QThread *cThread){
            qDebug() << "startScanSubdomain....";

            connect(cThread, &QThread::started, this, &brute::Scanner::lookupSubdomain);
            connect(this, &brute::Scanner::quitThread, cThread, &QThread::quit);
            connect(this, &brute::Scanner::next, this, &brute::Scanner::lookupSubdomain);
        }
        void startScanTLD(QThread *cThread){
            connect(cThread, &QThread::started, this, &brute::Scanner::lookupTLD);
            connect(this, &brute::Scanner::quitThread, cThread, &QThread::quit);
            connect(this, &brute::Scanner::next, this, &brute::Scanner::lookupTLD);
        }

    private slots:
        void lookupSubdomain();
        void lookupTLD();
        void lookupFinished();

    signals:
        void next(); // next lookup
        void result(QString subdomain, QString ip); // lookup results

    private:
        brute::ScanArgs *m_args;
        QDnsLookup *m_dns;
        QMutex mutex;
};

struct ReturnVal{
    bool lookup = false;
    bool next = false;
    bool quit = false;
};

ReturnVal lookupSubdomain(QDnsLookup*, ScanArgs*);
ReturnVal lookupTLD(QDnsLookup*, ScanArgs*);
}
#endif //BRUTE_H
