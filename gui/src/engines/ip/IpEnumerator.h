#ifndef IPENUMERATOR_H
#define IPENUMERATOR_H

#include "lib-ip.h"

/*********************************************************
                ACTIVE_SUBDOMAINS ENUMERATOR
**********************************************************/
class IpEnumerator : public QObject{
    Q_OBJECT

    private:
        int m_currentTargetToEnumerate = 0;
        //...
        ScanConfig *m_scanConfig;
        ip::ScanArguments *m_scanArguments;
        //...
        QHostInfo *hostInfo;

    public:
        IpEnumerator(ScanConfig *scanConfig, ip::ScanArguments *scanArguments);
        ~IpEnumerator();
        //...
        void enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void lookupFinished(QHostInfo info);

    signals:
        void performAnotherLookup();
        //...
        void scanResult(QString subdomain, QString ipAddress);
        void progress(int value);
        void scanLog(QString log);
        //...
        void quitThread();
};

#endif // IPENUMERATOR_H
