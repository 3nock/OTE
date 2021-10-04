#ifndef ACTIVESCANNER_H
#define ACTIVESCANNER_H

#include <QTcpSocket>
#include "../AbstractScanner.h"
#include "src/dialogs/ConfigDialog.h"


enum ACTIVE{
    DNS = 0,
    HTTP = 1,
    HTTPS = 2,
    FTP = 3,
    SMTP = 4
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
