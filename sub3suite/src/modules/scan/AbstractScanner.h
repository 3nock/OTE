#ifndef ABSTRACTSCANNER_H
#define ABSTRACTSCANNER_H

#include <QObject>
#include <QThread>
#include "src/utils/utils.h"


struct RetVal{
    bool lookup = false;
    bool next = false;
    bool quit = false;
};

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
        virtual void lookup()=0;

        /* quiting all running threads upon receiving stop signal... */
        void onStopScan(){
            emit quitThread();
        }

    signals:
        void quitThread();
        void infoLog(QString log);
        void errorLog(QString log);
        void scanProgress(int value);

        /* for certScanner... */
        void resultSHA1(QString);
        void resultSHA256(QString);
        void resultRaw(QByteArray);
        void resultSubdomain(QString);

        /* for banner... */
        void scanResultBanner(QString banner);
};

#endif // ABSTRACTSCANNER_H
