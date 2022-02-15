#ifndef PORTSCANNER_H
#define PORTSCANNER_H

#include "ActiveScanner.h"


namespace port {

class Scanner: public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(active::ScanArgs *args);
        ~Scanner() override;

    signals:
        void scanResult(s3s_struct::HOST host);

    private slots:
        void lookup() override;

    private:
        active::ScanArgs *m_args;
        QTcpSocket *m_socket;
        QString m_target;
};

QString getTarget(active::ScanArgs *args);

}

#endif // PORTSCANNER_H
