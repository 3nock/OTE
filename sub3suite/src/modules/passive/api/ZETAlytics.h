#ifndef ZETALYTICS_H
#define ZETALYTICS_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct ZETAlytics{
    QString name = OSINT_MODULE_ZETALYTICS;
    QString url = "https://zetalytics.com/";
    QString url_apiDoc = "https://zetalytics.com/";
    QString summary = "https://zetalytics.com/";

    QMap<QString, QStringList> flags = {{"Subdomain",
                                         {PLACEHOLDERTEXT_DOMAIN, "Subdomain"}}};
    QMap<int, QList<int>> input_output = {};
};
}

class ZETAlytics: public AbstractOsintModule{

    public:
        explicit ZETAlytics(ScanArgs args);
        ~ZETAlytics() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // ZETALYTICS_H
