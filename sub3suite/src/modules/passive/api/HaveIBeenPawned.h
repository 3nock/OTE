#ifndef HAVEIBEENPAWNED_H
#define HAVEIBEENPAWNED_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct HaveIBeenPawned{
    QString name = "HaveIBeenPawned";
    QString url = "https://haveibeenpwned.com";
    QString url_apiDoc = "https://haveibeenpwned.com/API/v3";
    QString summary = "Check if your email or phone is in a data breach";
    QMap<QString, QStringList> flags = {{"Breached Account",
                                         {PLACEHOLDERTEXT_EMAIL, "Getting all breaches for an account"}},
                                        {"Pastes Account",
                                         {PLACEHOLDERTEXT_EMAIL, "Getting all pastes for an account"}},
                                        {"Breached Site",
                                         {PLACEHOLDERTEXT_DOMAIN, "Getting all breached sites in the system"}},
                                        {"Single Breach",
                                         {"breach name", "Getting a single breached site"}}};

    QMap<int, QList<int>> input_output = {};
};
}

class HaveIBeenPawned: public AbstractOsintModule{

    public:
        HaveIBeenPawned(ScanArgs args);
        ~HaveIBeenPawned() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // HAVEIBEENPAWNED_H
