#ifndef LEVELENUMERATOR_H
#define LEVELENUMERATOR_H

#include "lib-level.h"

class LevelEnumerator: public QObject{
    Q_OBJECT

    private:
        int m_currentWordlistToEnumerate = 0;
        int m_currentTargetToEnumerate = 0;
        ScanConfig *m_scanConfig;
        ScanArguments_level *m_scanArguments;
        QDnsLookup *m_dns;

    public:
        LevelEnumerator(ScanConfig *scanConfig, ScanArguments_level *scanArguments);
        ~LevelEnumerator();
        //...
        void enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void lookupFinished();

    signals:
        void performAnotherLookup();
        //...
        void progress(int value);
        void scanResult(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        //...
        void quitThread();
};

#endif // LEVELENUMERATOR_H
