#ifndef DNSRECORDSSCANNER_H
#define DNSRECORDSSCANNER_H

#include "src/core.h"


enum OPTION{
    ALLRECORDS = 0,
    SRV = 1
};


namespace records{

struct ScanArguments{
    QListWidget *srvWordlist;
    QListWidget *targetList;
    //...
    int currentTargetToEnumerate;
    int currentSrvToEnumerate;
    int progress;
    //...
    bool RecordType_srv;
    bool RecordType_a;
    bool RecordType_aaaa;
    bool RecordType_mx;
    bool RecordType_ns;
    bool RecordType_txt;
    bool RecordType_cname;
};
typedef struct ScanArguments ScanArguments;

struct ScanResults{
    ResultsModel *resultsModel;
    //..
    QLabel *resultsCountLabel;
    QLabel *srvResultsLabel;
};
typedef struct ScanResults ScanResults;


class Scanner: public QObject{
    Q_OBJECT

    private:
        ScanConfig *m_scanConfig;
        records::ScanArguments *m_scanArguments;
        records::ScanResults *m_scanResults;
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
        //...
        int m_activeLookups = 0;
        bool m_firstToResolve = true;
        //...
        QString m_currentTarget;

    public:
        Scanner(ScanConfig *scanConfig, records::ScanArguments *scanArguments, records::ScanResults *scanResults);
        ~Scanner();
        //...
        void startScan(QThread *cThread);
        void startScan_srv(QThread *cThread);

    public slots:
        void stopScan();

    private slots:
        void lookup();
        void lookup_srv();
        //...
        void srvLookupFinished();
        void aLookupFinished();
        void aaaaLookupFinished();
        void mxLookupFinished();
        void cnameLookupFinished();
        void nsLookupFinished();
        void txtLookupFinished();

    signals:
        void scanProgress(int value);
        void scanLog(QString log);
        //...
        void quitThread();
        void done();
        //...
        void doLookup();
        void doLookup_srv();
};
}
#endif // DNSRECORDSSCANNER_H
