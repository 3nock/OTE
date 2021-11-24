#ifndef CERTSCANNER_H
#define CERTSCANNER_H

#include <QTcpSocket>
#include <QSslCertificate>
#include "AbstractScanner.h"
#include "src/dialogs/ConfigDialog.h"

namespace cert {

struct ScanArguments{
    QString target;
    //QString targetList;
    int timeout = 3;

    /* protocal to connect to */
    bool https = false;
    bool ssh = false;
    bool ftp = false;
};

class Scanner : public AbstractScanner{

    public:
        Scanner(cert::ScanArguments args);
        ~Scanner() override;

    private slots:
        void lookup() override;

    private:
        cert::ScanArguments m_args;
};
}

#endif // CERTSCANNER_H
