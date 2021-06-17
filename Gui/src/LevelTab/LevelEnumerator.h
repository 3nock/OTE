#ifndef LEVELENUMERATOR_H
#define LEVELENUMERATOR_H

#include "lib-level.h"

class LevelEnumerator: public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        int m_currentResultsItem = 0;
        ScanArguments_level* m_scanArguments;
        QDnsLookup* m_dns;

    public:
        LevelEnumerator(ScanArguments_level *scanArguments);
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
        void scanLog(QString log);
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void quitThread();
};

#endif // LEVELENUMERATOR_H
