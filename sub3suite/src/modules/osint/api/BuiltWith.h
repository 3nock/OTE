#ifndef BUILTWITH_H
#define BUILTWITH_H

#include "../AbstractOsintModule.h"

/*
 */
namespace ModuleInfo {
struct BuiltWith{
    QString name = "BuiltWith";
    QString url = "https://builtwith.com/";
    QString url_apiDoc = "https://api.builtwith.com/";
    QString summary = "Find out what websites are Built With";
    QMap<QString, QStringList> flags = {{"lookup",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns the technology that the domain is builtwith"}}};
};
}

class BuiltWith: public AbstractOsintModule{

    public:
        BuiltWith(ScanArgs *args);
        ~BuiltWith() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};
#endif // BUILTWITH_H
