#ifndef ACTIVESCANNER_H
#define ACTIVESCANNER_H

#include <QTcpSocket>
#include "AbstractScanner.h"
#include "src/dialogs/ActiveConfigDialog.h"


enum ACTIVE{
    DNS = 0,
    HTTP = 1,
    HTTPS = 2,
    FTP = 3,
    SMTP = 4
};

namespace active {

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
    active::ScanConfig *config;
    QStringList targetList;

    int progress;
    int currentTargetToEnumerate;

    quint16 service;
    bool checkActiveService;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(active::ScanArgs *args);
        ~Scanner() override;


    private slots:
        void lookupFinished();
        void lookup() override;

    signals:
        void anotherLookup();
        void scanResult(QString subdomain, QString ipAddress);

    private:
        int m_currentTargetToEnumerate = 0;
        active::ScanArgs *m_args;
        /* ... */
        QDnsLookup *m_dns;
        QTcpSocket *m_socket;
};
}
#endif // ACTIVESCANNER_H
