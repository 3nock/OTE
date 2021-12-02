#ifndef EMAILREP_H
#define EMAILREP_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct EmailRep{
    QString name = "EmailRep";
    QString url = "https://emailrep.io/";
    QString url_apiDoc = "https://docs.emailrep.io/";
    QString summary = "EmailRep is a system of crawlers, scanners and enrichment services that collects data on email addresses, domains, and internet personas.";

    QMap<QString, QStringList> flags = {{"Email Query",
                                         {PLACEHOLDERTEXT_EMAIL, "Query an email..."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class EmailRep: public AbstractOsintModule{

    public:
        EmailRep(ScanArgs *args);
        ~EmailRep() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // EMAILREP_H
