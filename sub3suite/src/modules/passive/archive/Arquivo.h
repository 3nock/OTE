#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Arquivo{
    QString name = OSINT_MODULE_ARQUIVO;
    QString url = "https://arquivo.pt/";
    QString url_apiDoc = "https://github.com/arquivo/pwa-technologies/wiki/APIs";
    QString summary = "Arquivo.pt is a research infrastructure that allows searching and accessing web pages archived since 1996. "
                      "The main objective is the preservation of information published on the web for research purposes.";

    QMap<QString, QStringList> flags = {{"TextSearch",
                                         {PLACEHOLDERTEXT_QUERY, "The Arquivo.pt TextSearch API allows full-text search and access preserved web content and related metadata. "
                                                               "It is also possible to search by URL, accessing all versions of preserved web content."}},
                                        {"CDX Server",
                                         {PLACEHOLDERTEXT_DOMAIN, "CDX-server API allows automatic access in order to list, sort, and filter preserved pages from a given URL."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_URL,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_QUERYTERM,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}


class Arquivo: public AbstractOsintModule{

    public:
        explicit Arquivo(ScanArgs args);
        ~Arquivo() override;

    public slots:
        void start() override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // ARQUIVO_H
