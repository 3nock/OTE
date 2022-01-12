#ifndef ZETALYTICS_H
#define ZETALYTICS_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct ZETAlytics{
    QString name = "ZETAlytics";
    QString url = "https://zetalytics.com/";
    QString url_apiDoc = "";
    QString summary = "https://zetalytics.com/";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {};
};
}

class ZETAlytics: public AbstractOsintModule{

    public:
        ZETAlytics(ScanArgs args);
        ~ZETAlytics() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // ZETALYTICS_H
