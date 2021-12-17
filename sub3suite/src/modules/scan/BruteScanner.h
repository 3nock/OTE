#ifndef BRUTESCANNER_H
#define BRUTESCANNER_H

#include "AbstractScanner.h"


namespace brute {

struct ScanConfig{
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    QStringList customNameServers;
    int threadsCount = 50;
    int timeout = 3000;

    bool checkWildcard = false;
    bool hasWildcard = false;
    QString wildcardIp;
};

struct ScanArgs{
    brute::ScanConfig *config;
    QStringList targetList;
    QStringList wordlist;

    bool tldBrute;
    bool subBrute;

    int currentWordlistToEnumerate;
    int currentTargetToEnumerate;
    int progress;
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
        void anotherLookup();
        void scanResult(QString subdomain, QString ipAddress, QString target);

    private:
        int m_currentWordlistToEnumerate = 0;
        int m_currentTargetToEnumerate = 0;
        brute::ScanArgs *m_args;
        QDnsLookup *m_dns;
};

}
#endif //BRUTE_H
