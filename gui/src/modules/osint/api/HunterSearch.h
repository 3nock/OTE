#ifndef HUNTERSEARCH_H
#define HUNTERSEARCH_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: email
 */

namespace ModuleInfo {
    struct HunterSearch{
        QString name = "Hunter";
        QString url = "https://hunter.io/";
        QString url_apiDoc = "";
        QString summary = "Hunter lets you find professional email addresses in seconds and \n"
                          "connect with the people that matter for your business.";

        QMap<QString, QStringList> flags = {{"Domain Search", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Email Finder", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Author Finder", {PLACEHOLDERTEXT_URL, ""}},
                                        {"Email Verifier", {PLACEHOLDERTEXT_EMAIL, ""}},
                                        {"Email Count", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Account Information", {PLACEHOLDERTEXT_NONE, ""}}};
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
