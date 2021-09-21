#ifndef VIRUSTOTAL_H
#define VIRUSTOTAL_H

#include "AbstractModule.h"

class VirusTotal: public AbstractModule{

    public:
        VirusTotal(QString target = nullptr, QObject *parent = nullptr);
        ~VirusTotal() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // VIRUSTOTAL_H
