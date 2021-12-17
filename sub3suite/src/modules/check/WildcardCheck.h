#ifndef WILDCARDCHECK_H
#define WILDCARDCHECK_H

#include <QObject>
#include "src/utils/utils.h"
#include "src/modules/scan/BruteScanner.h"

class WildcardCheck: public QObject {
    Q_OBJECT

    public:
        WildcardCheck(brute::ScanArgs *args);
        ~WildcardCheck();
        //...
        void enumerate(QThread *cThread);

    private slots:
        void onLookupFinished();
        void lookup();

    signals:
        void quitThread();

    private:
        brute::ScanArgs *m_args;
        QDnsLookup *m_dns;
};

#endif // WILDCARDCHECK_H
