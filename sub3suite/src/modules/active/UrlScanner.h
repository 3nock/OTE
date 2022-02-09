/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef URLSCANNER_H
#define URLSCANNER_H


#include <QTcpSocket>
#include <QMutex>
#include <QQueue>
#include "AbstractScanner.h"


namespace url {

struct ScanStat {  // scan statistics
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    int threads = 50;
    int timeout = 3000;

    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    url::ScanConfig *config;
    QQueue<QString> targets;
    int progress;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(url::ScanArgs *args);
        ~Scanner() override;


    private slots:
        void lookup() override;
        void lookupFinished();

    signals:
        void next(); // next lookup
        void scanResult(QString url);

    private:
        url::ScanArgs *m_args;
};
}

#endif // URLSCANNER_H
