/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ABSTRACTSCANNER_H
#define ABSTRACTSCANNER_H

#include <QObject>
#include <QThread>
#include <QSemaphore>
#include "src/utils/utils.h"


enum class RETVAL{
    NEXT_LEVEL,
    LOOKUP,
    NEXT,
    QUIT
};

namespace scan {
struct Log{
    QString target;
    QString message;
    QString nameserver;
    QString recordType;
};

}

class AbstractScanner : public QObject{
    Q_OBJECT

    public:
        AbstractScanner(QObject *parent = nullptr)
            : QObject(parent)
        {
        }
        /* start the scan Object... */
        void startScan(QThread *cThread)
        {
            connect(cThread, &QThread::started, this, &AbstractScanner::lookup);
            connect(this, &AbstractScanner::quitThread, cThread, &QThread::quit);
        }

    public slots:
        /* the main method that performs the lookup... */
        virtual void lookup() = 0;

        /* quiting all running threads upon receiving stop signal... */
        virtual void onStopScan(){
            emit quitThread();
        }

        /* use QSemaphore to pause and resume threads */
        virtual void onPauseScan(){
            semaphore.acquire();
        }

        virtual void onResumeScan(){
            semaphore.release();
        }

    signals:
        void quitThread();
        void scanLog(scan::Log log);
        void scanProgress(int value);

    private:
        QSemaphore semaphore;

    protected:
        scan::Log log;
};

#endif // ABSTRACTSCANNER_H
