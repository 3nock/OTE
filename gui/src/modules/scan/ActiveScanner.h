#ifndef ACTIVESCANNER_H
#define ACTIVESCANNER_H

#include <QTcpSocket>
#include "AbstractScanner.h"
#include "src/dialogs/ConfigDialog.h"


enum ACTIVE{
    DNS = 0,
    HTTP = 1,
    HTTPS = 2,
    FTP = 3,
    SMTP = 4
};

enum PORTS{
    /*
    Web
    80	HTTP
    443	SSL
    21	FTP
    990	FTPs
    22	SFTP / SSH
    3306	MySQL

    Email
    110	POP – Incoming
    995	POP SSL – Incoming
    143	IMAP – Incoming
    993	IMAP SSL – Incoming
    25, 80, 3535	SMTP – Outgoing
    465	SMTP SSL – Outgoing

    Cpanel
    2082	cPanel TCP inbound
    2083	cPanel SSL TCP inbound
    2086	WHM TCP inbound
    2087	WHM SSL TCP inbound
    2089	WHM SSL TCP inbound
    2095	Webmail TCP inbound
    2096	Webmail SSL TCP inbound
    */
};

namespace active {

struct ScanArguments{
    QStringList targetList;
    //...
    int progress;
    int currentTargetToEnumerate;
    //...
    quint16 service;
    bool checkActiveService;
};
typedef struct ScanArguments ScanArguments;


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(ScanConfig *scanConfig, active::ScanArguments *scanArguments);
        ~Scanner() override;


    private slots:
        void lookupFinished();
        void lookup() override;

    signals:
        void anotherLookup();
        void scanResult(QString subdomain, QString ipAddress);

    private:
        int m_currentTargetToEnumerate = 0;
        //...
        ScanConfig *m_scanConfig;
        active::ScanArguments *m_scanArguments;
        //...
        QDnsLookup *m_dns;
        QTcpSocket *m_socket;
};
}
#endif // ACTIVESCANNER_H
