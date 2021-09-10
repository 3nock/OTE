#ifndef OSINTENUMERATOR_H
#define OSINTENUMERATOR_H

#include "src/utils.h"
#include <QObject>
#include <QThread>
#include <QLabel>

namespace osint{

struct ScanArguments{
    QStringList choosenOptions;
    char* targetDomain;
};
typedef struct  ScanArguments ScanArguments_osint;

struct ScanResults{
    QLabel* label_subdomainsCount;
    QStandardItemModel* results_model;
    int* resultsCount;
};
typedef struct ScanResults ScanResults;


class Scanner: public QObject{
    Q_OBJECT

    public:
        Scanner(osint::ScanArguments *scanArguments, osint::ScanResults *scanResults);
        ~Scanner();
        void startScan(QThread *cThread);

    public slots:
        void stopScan();

    private slots:
        void lookup();

    signals:
        void scanLog(QString log);
        void scanResult(QString subdomain, QString ipAddress);
        //...
        void quitThread();

    private:
        osint::ScanArguments *m_scanArguments;
        osint::ScanResults *m_scanResults;
        bool m_stopEnumeration = false;
};
}
#endif // OSINTENUMERATOR_H
