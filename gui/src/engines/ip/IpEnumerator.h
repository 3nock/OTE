#ifndef IPENUMERATOR_H
#define IPENUMERATOR_H

#include "src/core.h"

/********************************************************
                        structures
*********************************************************/
struct ScanArguments_Ip{
    QListWidget *targetList;
    QLabel *label_resultsCount;
    QStandardItemModel *model_results;
    //...
    int progress;
    int currentTargetToEnumerate;
};
typedef struct ScanArguments_Ip ScanArguments_Ip;


/*********************************************************
                ACTIVE_SUBDOMAINS ENUMERATOR
**********************************************************/
class IpEnumerator : public QObject{
    Q_OBJECT

    private:
        int m_currentTargetToEnumerate = 0;
        //...
        ScanConfig *m_scanConfig;
        ScanArguments_Ip *m_scanArguments;
        //...
        QHostInfo *hostInfo;

    public:
        IpEnumerator(ScanConfig *scanConfig, ScanArguments_Ip *scanArguments);
        ~IpEnumerator();
        //...
        void enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void lookupFinished(QHostInfo info);

    signals:
        void performAnotherLookup();
        //...
        void scanResult(QString subdomain, QString ipAddress);
        void progress(int value);
        void scanLog(QString log);
        //...
        void quitThread();
};

#endif // IPENUMERATOR_H
