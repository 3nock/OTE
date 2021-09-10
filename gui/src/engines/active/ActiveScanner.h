#ifndef ACTIVESCANNER_H
#define ACTIVESCANNER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "src/utils.h"
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
    QListWidget *targetList;
    //...
    int progress;
    int currentTargetToEnumerate;
    //...
    quint16 service;
    bool checkActiveService;
};
typedef struct ScanArguments ScanArguments;


class Scanner : public QObject{
    Q_OBJECT

    public:
        Scanner(ScanConfig *scanConfig, active::ScanArguments *scanArguments);
        ~Scanner();
        //...
        void startScan(QThread *cThread);

    public slots:
        void stopScan();

    private slots:
        void lookup();
        void lookupFinished();

    signals:
        void anotherLookup();
        //...
        void scanProgress(int value);
        void scanResult(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        //...
        void quitThread();

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
