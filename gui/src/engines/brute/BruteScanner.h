#ifndef BRUTESCANNER_H
#define BRUTESCANNER_H

#include <QObject>
#include <QThread>
#include "src/utils.h"
#include "src/dialogs/ConfigDialog.h"
#include "src/dialogs/WordlistDialog.h"
#include "src/dialogs/WordlistGeneratorDialog.h"


namespace brute {

struct ScanArguments{
    QStringList targetList;
    QListWidget *wordlist;
    //...
    bool tldBrute;
    bool subBrute;
    //...
    int currentWordlistToEnumerate;
    int currentTargetToEnumerate;
    int progress;
};
typedef ScanArguments ScanArguments;


class Scanner : public QObject{
    Q_OBJECT

    public:
        Scanner(ScanConfig *scanConfig, brute::ScanArguments *scanArguments);
        ~Scanner();
        //...
        void startScan(QThread *cThread);

    public slots:
        void stopScan();

    private slots:
        void lookup();
        void lookupFinished();

    signals:
        void anotherLookup();
        //...
        void scanProgress(int value);
        void scanResult(QString subdomain, QString ipAddress, QString target);
        void scanLog(QString log);
        //...
        void quitThread();

    private:
        int m_currentWordlistToEnumerate = 0;
        int m_currentTargetToEnumerate = 0;
        //...
        brute::ScanArguments *m_scanArguments;
        ScanConfig *m_scanConfig;
        QDnsLookup *m_dns;
};
}
#endif //BRUTE_H
