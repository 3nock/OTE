#ifndef SUBLIST3R_H
#define SUBLIST3R_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Sublist3r{
    QString name = OSINT_MODULE_SUBLIST3R;
    QString url = "https://api.sublist3r.com/";
    QString url_apiDoc = "";
    QString summary = "Sublist3r";
    QMap<QString, QStringList> flags = {{"subdomains",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns list of all available subdomains"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN, {OUT_SUBDOMAIN}}};
};
}

class Sublist3r: public AbstractOsintModule{

    public:
        explicit Sublist3r(ScanArgs args);
        ~Sublist3r() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;
};

#endif // SUBLIST3R_H
