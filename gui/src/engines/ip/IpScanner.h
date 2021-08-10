#ifndef IPENUMERATOR_H
#define IPENUMERATOR_H

#include "src/core.h"


namespace ip{

struct ScanArguments{
    QListWidget *targetList;
    QLabel *label_resultsCount;
    QStandardItemModel *model_results;
    //...
    int progress;
    int currentTargetToEnumerate;
};
typedef struct ScanArguments ScanArguments;


class Scanner : public QObject{
    Q_OBJECT

    private:
        int m_currentTargetToEnumerate = 0;
        //...
        ScanConfig *m_scanConfig;
        ip::ScanArguments *m_scanArguments;
        //...
        QHostInfo *hostInfo;

    public:
        Scanner(ScanConfig *scanConfig, ip::ScanArguments *scanArguments);
        ~Scanner();
        //...
        void startScan(QThread *cThread);

    public slots:
        void stopScan();

    private slots:
        void lookup();
        void lookupFinished(QHostInfo info);

    signals:
        void anotherLookup();
        //...
        void scanResult(QString subdomain, QString ipAddress);
        void scanProgress(int value);
        void scanLog(QString log);
        //...
        void quitThread();
};
}
#endif // IPENUMERATOR_H
