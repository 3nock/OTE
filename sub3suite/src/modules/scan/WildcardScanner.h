/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef WILDCARDSCANNER_H
#define WILDCARDSCANNER_H

#include <QObject>
#include "src/utils/utils.h"
#include "src/modules/scan/BruteScanner.h"

class WildcardScanner: public QObject {
    Q_OBJECT

    public:
        WildcardScanner(brute::ScanArgs *args);
        ~WildcardScanner();
		
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

#endif // WILDCARDSCANNER_H
