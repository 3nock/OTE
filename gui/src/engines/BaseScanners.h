#ifndef BASESCANNERS_H
#define BASESCANNERS_H

#include "src/core.h"
#include "src/engines/brute/lib-brute.h"

/***************************************************************************************
                                      GET NameServers
****************************************************************************************/

/****************************************************************************************
                                      CHECK WILDCARDS
*****************************************************************************************/
class BruteEnumerator_Wildcards: public QObject{
    Q_OBJECT

    private:
        brute::ScanArguments *m_scanArguments;
        ScanConfig *m_scanConfig;
        QDnsLookup *m_dns;

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
};

#endif // BASESCANNERS_H
