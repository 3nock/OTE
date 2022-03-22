#ifndef SPAMHAUS_H
#define SPAMHAUS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct SpamHaus{
    QString name = OSINT_MODULE_SPAMHAUS;
    QString url = "https://spamhaus.com/";
    QString url_apiDoc = "https://docs.spamhaus.com/";
    QString summary = "Protect and investigate using IP and domain reputation data.";
    QMap<QString, QStringList> flags = {{"Forward Search",
                                         {PLACEHOLDERTEXT_DOMAIN, "Forward search..."}},
                                        {"Reverse Search",
                                         {PLACEHOLDERTEXT_CIDR, "Reverse search..."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class SpamHaus: public AbstractOsintModule{

    public:
        SpamHaus(ScanArgs args);
        ~SpamHaus() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // SPAMHAUS_H
