#ifndef OSINT_H
#define OSINT_H

#include "src/utils.h"
#include "src/engines/Base.h"
#include "OsintScanner.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ConfigDialog.h"

// osint engines...
#define OSINT_VIRUSTOTALAPI "virustotalapi"
#define OSINT_OMNISINT "omnisint"
#define OSINT_QWANT "qwant"
#define OSINT_URLSCAN "urlscan"
#define OSINT_RAPIDDNS "rapiddns"
#define OSINT_PENTESTTOOLS "pentesttools"
#define OSINT_PROJECTDISCOVERY "projectdiscovery"
#define OSINT_SPYSE "spyse"
#define OSINT_THREATMINER "threatminer"
#define OSINT_SHODAN "shodan"
#define OSINT_GOOGLE "google"
#define OSINT_BING "bing"
#define OSINT_GITHUB "github"
#define OSINT_CENSYS "censys"
#define OSINT_SECURITYTRAILS "securitytrails"
#define OSINT_CLOUDFLARE "cloudflare"
#define OSINT_INTELX "intelx"
#define OSINT_VIRUSTOTAL "virustotal"
#define OSINT_CERTSPOTTER "certspotter"
#define OSINT_CRTSH "crtsh"
#define OSINT_DOGPILE "dogpile"
#define OSINT_DUCKDUCKGO "duckduckgo"
#define OSINT_EXALEAD "exalead"
#define OSINT_HUNTERSEARCH "huntersearch"
#define OSINT_NETCRAFT "netcraft"
#define OSINT_OTX "otx"
#define OSINT_SUIP "suip"
#define OSINT_TRELLO "trello"
#define OSINT_SAN "san"
#define OSINT_THREATCROWD "threatcrowd"
#define OSINT_DNSBUFFEROVERRUN "dnsbufferoverrun"
#define OSINT_HACKERTARGET "hackertarget"
#define OSINT_PKEY "pkey"
#define OSINT_WAYBACKMACHINE "waybackmachine"
#define OSINT_ASK "ask"
#define OSINT_BAIDU "baidu"
#define OSINT_DNSDUMPSTER "dnsdumpster"
#define OSINT_PASSIVEDNS "passivedns"
#define OSINT_YAHOO "yahoo"

namespace Ui {
    class Osint;
}

class Osint : public BaseClass{
    Q_OBJECT

    public:
        Osint(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Osint();

    private slots:
        void on_buttonStart_clicked();
        void on_buttonClearResults_clicked();
        void on_buttonAction_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_buttonKeys_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        //...
        void on_checkBoxUseProfiles_clicked(bool checked);
        void on_buttonLoadProfile_clicked();
        void on_buttonDeleteProfile_clicked();
        void on_buttonNewProfile_clicked();
        //...
        void onEnumerationComplete();
        void on_checkBoxMultipleTargets_clicked(bool checked);

    private:
        Ui::Osint *ui;
        //...
        void getUserOptions(QStringList *choosenOptions);
        void initProfiles();
        //...
        osint::ScanArguments *m_scanArguments;
        osint::ScanResults *m_scanResults;
        //...
        int m_subdomainsCount = 0;
        //...
        QString currentPath;
        QString m_targetDomain;
};

#endif // OSINT_H
