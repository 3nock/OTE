#ifndef BASESCANNERS_H
#define BASESCANNERS_H

#include "src/utils/utils.h"
#include "src/engines/brute/BruteScanner.h"

/***************************************************************************************
                                  Is Target Active
****************************************************************************************/
class TargetCheck: public QObject {
    Q_OBJECT

    public:
        TargetCheck(QObject *parent = nullptr);

    signals:
        void anotherLookup();
        //...
        void isActive();
        void notActive();

    private slots:
        void lookupFinished();
        void lookup();
        void isTargetActive(QString target);

    private:
        int m_check = 0;
        QDnsLookup *m_dns;
};

/***************************************************************************************
                                  GET NameServers
****************************************************************************************/

/****************************************************************************************
                                   CHECK WILDCARDS
*****************************************************************************************/
class BruteEnumerator_Wildcards: public QObject {
    Q_OBJECT

    public:
        BruteEnumerator_Wildcards(ScanConfig *scanConfig, brute::ScanArguments *scanArguments);
        ~BruteEnumerator_Wildcards();
        //...
        void enumerate(QThread *cThread);

    private slots:
        void onLookupFinished();
        void lookup();

    signals:
        void quitThread();

    private:
        brute::ScanArguments *m_scanArguments;
        ScanConfig *m_scanConfig;
        QDnsLookup *m_dns;
};

#endif // BASESCANNERS_H
