#ifndef VIRUSTOTAL_H
#define VIRUSTOTAL_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct VirusTotal{
        QString name = "VirusTotal";
        QString url = "https://www.virustotal.com/";
        QString summary = "Analyze suspicious files and URLs to detect types of malware, \n"
                          "automaticsubdomainIpy share them with the security community";
        QMap<QString, QString> flags = {{"urls", "url"},
                                        {"domains", "domain name"},
                                        {"resolution", "domain name/ip-address"},
                                        {"ip-addresses", "ip-addresses"}};
    };
}

class VirusTotal: public AbstractOsintModule{

    public:
        VirusTotal(ScanArgs *args);
        ~VirusTotal() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key;
};

#endif // VIRUSTOTAL_H
