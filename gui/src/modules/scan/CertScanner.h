#ifndef CERTSCANNER_H
#define CERTSCANNER_H

#include <QTcpSocket>
#include <QSslCertificate>
#include "AbstractScanner.h"
#include "src/dialogs/ConfigDialog.h"

namespace cert {

class Scanner : public AbstractScanner{

    public:
        Scanner(QString target);
        ~Scanner() override;

    private slots:
        void lookup() override;

    private:
        QString m_target;
};
}
#endif // CERTSCANNER_H
