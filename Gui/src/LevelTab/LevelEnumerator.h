#ifndef LEVELENUMERATOR_H
#define LEVELENUMERATOR_H

#include "lib-level.h"

class LevelEnumerator: public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        ScanArguments_level* m_scanArguments;
        ScanResults_level* m_scanResults;
        QDnsLookup* m_dns;

    public:
        LevelEnumerator(ScanArguments_level *scanArguments, ScanResults_level *scanResults);
        ~LevelEnumerator();
        //...
        void Enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void onLookupFinished();

    signals:
        void performAnotherLookup();
        //...
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};

#endif // LEVELENUMERATOR_H
