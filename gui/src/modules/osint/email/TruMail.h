#ifndef TRUMAIL_H
#define TRUMAIL_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct TruMail{
        QString name = "TruMail";
        QString url = "https://trumail.io/";
        QString url_apiDoc = "https://trumail.io/documentation";
        QString summary = "Prevent bounced emails and low-quality users with FREE professional grade email verification";

        QMap<QString, QStringList> flags = {{"Email",
                                             {PLACEHOLDERTEXT_EMAIL, "Perform a single lookup on our real-time email verification"}}};
    };
}

class TruMail: public AbstractOsintModule{

    public:
        TruMail(ScanArgs *args);
        ~TruMail() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // TRUMAIL_H
