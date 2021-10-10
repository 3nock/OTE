#ifndef VIRUSTOTAL_H
#define VIRUSTOTAL_H

#include "../AbstractOsintModule.h"

class VirusTotal: public AbstractOsintModule{

    public:
        VirusTotal(ScanArgs *args);
        ~VirusTotal() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // VIRUSTOTAL_H
