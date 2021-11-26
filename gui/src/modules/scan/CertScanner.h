#ifndef CERTSCANNER_H
#define CERTSCANNER_H

#include <QSslCertificate>
#include "AbstractScanner.h"
#include "src/dialogs/ConfigDialog.h"

namespace certificate {
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
        Scanner(certificate::ScanArguments args);
        ~Scanner() override;

    private slots:
        void lookup() override;

    private:
        certificate::ScanArguments m_args;
};
}

#endif // CERTSCANNER_H
