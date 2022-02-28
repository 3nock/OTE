/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef URLSCANNER_H
#define URLSCANNER_H


#include "AbstractScanner.h"
#include "src/items/URLItem.h"

#include <QNetworkAccessManager>
#include <QMutex>
#include <QQueue>


namespace url {

struct ScanStat {  // scan statistics
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    int threads = 50;
    int timeout = 1000;

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
        void lookupFinished(QNetworkReply *reply);

    signals:
        void next();
        void scanResult(s3s_struct::URL);

    private:
        url::ScanArgs *m_args;
        QNetworkAccessManager *m_manager;
};

RETVAL getTarget(url::ScanArgs *args, QUrl &url);

}

#endif // URLSCANNER_H
