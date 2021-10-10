#ifndef WILDCARDCHECK_H
#define WILDCARDCHECK_H

#include <QObject>
#include "src/utils/utils.h"
#include "src/engines/brute/BruteScanner.h"

class WildcardCheck: public QObject {
    Q_OBJECT

    public:
        WildcardCheck(ScanConfig *scanConfig, brute::ScanArguments *scanArguments);
        ~WildcardCheck();
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
#endif // WILDCARDCHECK_H
