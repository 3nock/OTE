#ifndef OSINTENUMERATOR_H
#define OSINTENUMERATOR_H

#include "lib-osint.h"

/***************************** ENUMERATOR CLASS *******************************/
class Enumerator: public QObject{
    Q_OBJECT

    private:
        osint::ScanArguments *m_scanArguments;
        osint::ScanResults *m_scanResults;
        //...
        bool m_stopEnumeration = false;

    public:
        Enumerator(osint::ScanArguments *scanArguments, osint::ScanResults *scanResults);
        ~Enumerator();
        void Enumerate(QThread *cThread);

    public slots:
        void onStopEnumeration();

    private slots:
        void worker();

    signals:
        void scanLogs(QString log);
        void enumerationComplete();
        //...
        void quitThread();
};

#endif // OSINTENUMERATOR_H
