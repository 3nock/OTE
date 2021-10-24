#ifndef HUNTERSEARCH_H
#define HUNTERSEARCH_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct HunterSearch{
        QString name = "Hunter";
        QString url = "https://hunter.io/";
        QString summary = "Hunter lets you find professional email addresses in seconds and \n"
                          "connect with the people that matter for your business.";

        QMap<QString, QString> flags = {{"Domain Search", "domain name"},
                                        {"Email Finder", "domain name"},
                                        {"Author Finder", "url"},
                                        {"Email Verifier", "email"},
                                        {"Email Count", "domain name"},
                                        {"Account Information", ""}};
    };
}

class HunterSearch: public AbstractOsintModule{

    public:
        HunterSearch(ScanArgs *args);
        ~HunterSearch() override;
        //...
        bool verifyEmail(QString email);

    public slots:
        void start() override;
        void replyFinishedEmail(QNetworkReply *) override;

    private:
        QString m_key;
};

#endif // HUNTERSEARCH_H
