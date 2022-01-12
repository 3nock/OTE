#ifndef BOTSCOUT_H
#define BOTSCOUT_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct BotScout{
    QString name = "BotScout";
    QString url = "http://botscout.com";
    QString url_apiDoc = "http://botscout.com/api.htm";
    QString summary = "BotScout helps prevent automated web scripts, known as bots, from registering on forums, polluting databases, spreading spam, and abusing forms on web sites. "
                      "We do this by tracking the names, IPs, and email addresses that bots use and logging them as unique signatures for future reference.";

    QMap<QString, QStringList> flags = {{"EMAIL",
                                         {PLACEHOLDERTEXT_EMAIL, "Check an email address."}},
                                        {"IP",
                                         {PLACEHOLDERTEXT_IP, "Check an IP Address."}},
                                        {"NAME",
                                         {PLACEHOLDERTEXT_NAME, "Check a user name."}},
                                        {"ALL",
                                         {PLACEHOLDERTEXT_QUERY, "Check all."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class BotScout: public AbstractOsintModule{

    public:
        BotScout(ScanArgs args);
        ~BotScout() override;

    public slots:
        void start() override;
};

#endif // BOTSCOUT_H
