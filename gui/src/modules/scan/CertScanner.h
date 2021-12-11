#ifndef CERTSCANNER_H
#define CERTSCANNER_H

#include <QStack>
#include <QSslCertificate>
#include "AbstractScanner.h"
#include "src/dialogs/ActiveConfigDialog.h"

namespace certificate {

struct ScanArguments{
    QString target;
    QStack<QString> targetList;
    bool singleTarget = false;

    /* result type */
    bool raw = false;
    bool sha1 = false;
    bool sha256 = false;
    bool subdomain = false;

    /* protocal to connect to */
    bool https = false;
    bool ssh = false;
    bool ftp = false;

    /* ... */
    int timeout = 3;
    int progress = 0;
};

class Scanner : public AbstractScanner{

    public:
        Scanner(certificate::ScanArguments *args);
        ~Scanner() override;

    private slots:
        void lookup() override;

    private:
        certificate::ScanArguments *m_args;
};
}

#endif // CERTSCANNER_H
