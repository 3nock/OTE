#ifndef WHATCMS_H
#define WHATCMS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Whatcms{
    QString name = OSINT_MODULE_WHATCMS;
    QString url = "https://whatcms.org/";
    QString url_apiDoc = "https://whatcms.org/Documentation";
    QString summary = "WhatCMS.org handles requests from users around the world seeking to find out more about the websites they're using.  "
                      "There are hundreds of content management systems in use on the web with significantly different amounts of usage.";

    QMap<QString, QStringList> flags = {{"Technology Detection",
                                         {PLACEHOLDERTEXT_HOSTNAME, "The technology endpoint provides a more complete picture of the software powering webpages. "
                                                                    "In addition to detecting the CMS, this endpoint will return the programming language, database, web server and more of the requested url."}},
                                        {"Technology List",
                                         {PLACEHOLDERTEXT_NONE, "List Of Technologies."}},
                                        {"CMS Detection",
                                         {PLACEHOLDERTEXT_HOSTNAME, "Fetch WhatCMS.org data with curl, guzzle or your favorite HTTP client."}},
                                        {"WordPress Theme Detection",
                                         {PLACEHOLDERTEXT_HOSTNAME, "Fetch ThemeDetect.com data with curl, guzzle or your favorite HTTP client."}},
                                        {"Host Detection",
                                         {PLACEHOLDERTEXT_HOSTNAME, "Fetch Who-Hosts-This.com data with curl, guzzle or your favorite HTTP client."}},
                                        {"Status Endpoint",
                                         {PLACEHOLDERTEXT_NONE, "You can use the status endpoint for checking your account totals."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class Whatcms: public AbstractOsintModule{

    public:
        explicit Whatcms(ScanArgs args);
        ~Whatcms() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // WHATCMS_H
