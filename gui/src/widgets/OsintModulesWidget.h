#ifndef OSINTMODULESWIDGET_H
#define OSINTMODULESWIDGET_H

#include <QWidget>

namespace Ui {
class OsintModulesWidget;
}

struct OsintModules{
    bool certspotter = false;
    bool otx = false;
    bool sublist3r = false;
    bool threatminer = false;
    bool threatcrowd = false;
    bool hackertarget = false;
    bool dnsbufferoverrun = false;
    bool anubis = false;
    bool projectdiscovery = false;
    bool spyse = false;
    bool crtsh = false;
    bool dnsdumpster = false;
    bool suip = false;
    bool pkey = false;
    bool netcraft = false;
    bool omnisint = false;
    bool rapiddns = false;
    bool googleCert = false;
    bool qwant = false;
    bool urlscan = false;
    bool virustotalapi = false;
    bool waybackmachine = false;
    bool archivetoday = false;
    bool archiveit = false;
    bool bgpview = false;
    bool censysFree = false;
    bool c99 = false;
    bool commoncrawl = false;
    bool binaryEdge = false;
    bool huntersearch = false;
    bool ipinfo = false;
    bool github = false;
    bool mnemonic = false;
    bool robtex = false;
    bool riskiq = false;
    bool securitytrails = false;
    bool threatbook = false;
    bool shodan = false;
    bool whoisxmlapi = false;
    bool zetalytics = false;
    bool zoomeye = false;
    bool ipapi = false;
    bool ask = false;
    bool duckduckgo = false;
    bool baidu = false;
    bool dogpile = false;
    bool exalead = false;
    bool trello = false;
    bool yahoo = false;
    bool pagesinventory = false;
    bool bing = false;
    bool sitedossier = false;
    bool viewDns = false;
    bool intelx = false;
    bool cloudflare = false;
    bool google = false;
    bool pentesttools = false;
};

class OsintModulesWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit OsintModulesWidget(QWidget *parent = nullptr);
        ~OsintModulesWidget();
        //...
        QString currentPath;
        void getChoosenModules(OsintModules&);

    private slots:
        void on_useProfiles_clicked(bool checked);
        void on_buttonLoadProfile_clicked();
        void on_buttonCreateProfile_clicked();
        void on_buttonDeleteProfile_clicked();

    private:
        Ui::OsintModulesWidget *ui;
        void initProfiles();
};

#endif // OSINTMODULESWIDGET_H
