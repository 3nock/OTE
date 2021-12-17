#ifndef DNSRECORDSSCANNER_H
#define DNSRECORDSSCANNER_H

#include "AbstractScanner.h"


enum OPTION{
    ALLRECORDS = 0,
    SRV = 1
};


namespace records{

struct ScanConfig{
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    QStringList customNameServers;
    int threadsCount = 50;
    int timeout = 3000;

    bool checkWildcard = false;
    bool hasWildcard = false;
};

struct ScanArgs{
    records::ScanConfig *config;
    QStringList srvWordlist;
    QStringList targetList;

    int currentTargetToEnumerate;
    int currentSrvToEnumerate;
    int progress;

    bool RecordType_srv;
    bool RecordType_a;
    bool RecordType_aaaa;
    bool RecordType_mx;
    bool RecordType_ns;
    bool RecordType_txt;
    bool RecordType_cname;

    int count;
};

struct Results{
    QString domain;

    QStringList A;
    QStringList AAAA;
    QStringList MX;
    QStringList NS;
    QStringList TXT;
    QStringList CNAME;

    int srvPort = NULL;
    QString srvName = nullptr;
    QString srvTarget = nullptr;
};

class Scanner: public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(records::ScanArgs *args);
        ~Scanner() override;
        //...
        void startScan_srv(QThread *cThread);

    private slots:
        void lookup() override;
        void lookup_srv();
        void finish();
        //...
        void srvLookupFinished();
        void aLookupFinished();
        void aaaaLookupFinished();
        void mxLookupFinished();
        void cnameLookupFinished();
        void nsLookupFinished();
        void txtLookupFinished();

    signals:
        void scanResult(records::Results);
        void done();
        //...
        void doLookup();
        void doLookup_srv();

    private:
        records::ScanArgs *m_args;
        records::Results m_results;
        //...
        QStandardItem *m_dnsNameItem;
        QStandardItem *m_recordItem;
        //...
        QDnsLookup *m_dns_srv;
        QDnsLookup *m_dns_a;
        QDnsLookup *m_dns_aaaa;
        QDnsLookup *m_dns_mx;
        QDnsLookup *m_dns_ns;
        QDnsLookup *m_dns_txt;
        QDnsLookup *m_dns_cname;
        //...
        int m_currentTargetToEnumerate = 0;
        int m_currentSrvToEnumerate = 0;
        QString m_currentTarget;
        //...
        int m_activeLookups = 0;
        bool hasAtleastOneRecord = false;
};
}
#endif // DNSRECORDSSCANNER_H
