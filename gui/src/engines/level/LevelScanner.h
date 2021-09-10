#ifndef LEVELSCANNER_H
#define LEVELSCANNER_H

#include <QObject>
#include <QThread>
#include "src/utils.h"
#include "src/dialogs/ConfigDialog.h"
#include "src/dialogs/WordlistDialog.h"


namespace level {

struct ScanArguments{
    QListWidget* wordlist;
    QListWidget* targetList;
    //...
    int progress;
    int currentTargetToEnumerate;
    int currentWordlistToEnumerate;
    //...
    int currentLevel;
    int maxLevel;
};
typedef ScanArguments ScanArguments;


class Scanner: public QObject{
    Q_OBJECT

    public:
        Scanner(ScanConfig *scanConfig, level::ScanArguments *scanArguments);
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
        void scanResult(QString subdomain, QString ipAddress);
        void scanProgress(int value);
        void scanLog(QString log);
        //...
        void quitThread();

    private:
        int m_currentWordlistToEnumerate = 0;
        int m_currentTargetToEnumerate = 0;
        ScanConfig *m_scanConfig;
        level::ScanArguments *m_scanArguments;
        QDnsLookup *m_dns;
};
}
#endif // LEVELSCANNER_H
