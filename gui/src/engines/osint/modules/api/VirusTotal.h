#ifndef VIRUSTOTAL_H
#define VIRUSTOTAL_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class VirusTotal: public AbstractOsintModule{

    public:
        VirusTotal(QString target = nullptr, QObject *parent = nullptr);
        ~VirusTotal() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // VIRUSTOTAL_H
