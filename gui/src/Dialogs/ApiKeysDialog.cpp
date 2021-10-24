#include "ApiKeysDialog.h"
#include "ui_ApiKeysDialog.h"
//...
#include "src/utils/Config.h"
#include <QUrl>
#include <QDesktopServices>

/*
 * TODO: fix hunter..
 *       add more api keys..
 *       modify such that more info can be shown eg(api csubdomainIps left)
 *       save the urls in the .ini file instead of hardcoding them...
 *       also add a widget to display those url so they can be modifiable
 */

ApiKeysDialog::ApiKeysDialog(QWidget *parent): QDialog(parent),ui(new Ui::ApiKeysDialog)
{
    ui->setupUi(this);
    loadApiKeys();
}
ApiKeysDialog::~ApiKeysDialog(){
    delete ui;
}

void ApiKeysDialog::on_buttonSave_clicked(){
    Config::generalConfig().beginGroup("api-keys");
    //....
    Config::generalConfig().setValue("bing", ui->lineEditBing->text());
    Config::generalConfig().setValue("censys_id", ui->lineEditCensysId->text());
    Config::generalConfig().setValue("censys_secret", ui->lineEditCensysSecret->text());
    Config::generalConfig().setValue("certspotter", ui->lineEditCertspotter->text());
    Config::generalConfig().setValue("cloudflare", ui->lineEditCloudflare->text());
    Config::generalConfig().setValue("github", ui->lineEditGithub->text());
    Config::generalConfig().setValue("google", ui->lineEditGoogle->text());
    Config::generalConfig().setValue("hunter", ui->lineEditHunter->text());
    Config::generalConfig().setValue("huntersearch", ui->lineEditHunter->text());
    Config::generalConfig().setValue("intelx", ui->lineEditIntelX->text());
    Config::generalConfig().setValue("pentesttools", ui->lineEditPentestTools->text());
    Config::generalConfig().setValue("projectdiscovery", ui->lineEditProjectDiscovery->text());
    Config::generalConfig().setValue("riskiq_key", ui->lineEditRiskIqKey->text());
    Config::generalConfig().setValue("riskiq_username", ui->lineEditRiskIqUsername->text());
    Config::generalConfig().setValue("securitytrails", ui->lineEditSecurityTrails->text());
    Config::generalConfig().setValue("shodan", ui->lineEditShodan->text());
    Config::generalConfig().setValue("spyse", ui->lineEditSpyse->text());
    Config::generalConfig().setValue("virustotalapi", ui->lineEditVirusTotal->text());
    Config::generalConfig().setValue("binaryedge", ui->lineEditBinaryEdge->text());
    Config::generalConfig().setValue("c99", ui->lineEditC99->text());
    Config::generalConfig().setValue("ipapi", ui->lineEditIpApi->text());
    Config::generalConfig().setValue("ipinfo", ui->lineEditIpInfo->text());
    Config::generalConfig().setValue("threatbook", ui->lineEditThreatBook->text());
    Config::generalConfig().setValue("whoisxmlapi", ui->lineEditWhoisXmlApi->text());
    Config::generalConfig().setValue("zoomeye", ui->lineEditZoomEye->text());
    Config::generalConfig().setValue("viewdns", ui->lineEditViewDns->text());
    Config::generalConfig().setValue("hackertarget", ui->lineEditHackerTarget->text());
    Config::generalConfig().setValue("webresolver", ui->lineEditWebResolver->text());
    Config::generalConfig().setValue("circlUser", ui->lineEditCirclUser->text());
    Config::generalConfig().setValue("circlPwd", ui->lineEditCirclPwd->text());
    Config::generalConfig().setValue("mnemonic", ui->lineEditMnemonic->text());
    //...
    Config::generalConfig().endGroup();
    accept();
}

void ApiKeysDialog::on_buttonCancel_clicked(){
    accept();
}

void ApiKeysDialog::loadApiKeys(){
    QString key;
    Config::generalConfig().beginGroup("api-keys");
    ///
    /// loading the keys....
    ///
    key= Config::generalConfig().value("bing").toString();
    if(!key.isEmpty()){
        ui->lineEditBing->setText(key);
        ui->buttonGetBing->hide();
    }
    key = Config::generalConfig().value("censys_id").toString();
    if(!key.isEmpty()){
        ui->lineEditCensysId->setText(key);
        ui->buttonGetCensys->hide();
    }
    key = Config::generalConfig().value("censys_secret").toString();
    if(!key.isEmpty()){
        ui->lineEditCensysSecret->setText(key);
        ui->buttonGetCensys->hide();
    }
    key = Config::generalConfig().value("certspotter").toString();
    if(!key.isEmpty()){
        ui->lineEditCertspotter->setText(key);
        ui->buttonGetCertspotter->hide();
    }
    key = Config::generalConfig().value("cloudflare").toString();
    if(!key.isEmpty()){
        ui->lineEditCloudflare->setText(key);
        ui->buttonGetCloudflare->hide();
    }
    key = Config::generalConfig().value("github").toString();
    if(!key.isEmpty()){
        ui->lineEditGithub->setText(key);
        ui->buttonGetGithub->hide();
    }
    key = Config::generalConfig().value("google").toString();
    if(!key.isEmpty()){
        ui->lineEditGoogle->setText(key);
        ui->buttonGetGoogle->hide();
    }
    key = Config::generalConfig().value("hunter").toString();
    if(!key.isEmpty()){
        ui->lineEditHunter->setText(key);
        ui->buttonGetHunter->hide();
    }
    key = Config::generalConfig().value("huntersearch").toString();
    if(!key.isEmpty()){
        ui->lineEditHunter->setText(key);
        ui->buttonGetHunter->hide();
    }
    key = Config::generalConfig().value("intelx").toString();
    if(!key.isEmpty()){
        ui->lineEditIntelX->setText(key);
        ui->buttonGetIntelX->hide();
    }
    key = Config::generalConfig().value("pentesttools").toString();
    if(!key.isEmpty()){
        ui->lineEditPentestTools->setText(key);
        ui->buttonGetPentestTools->hide();
    }
    key = Config::generalConfig().value("projectdiscovery").toString();
    if(!key.isEmpty()){
        ui->lineEditProjectDiscovery->setText(key);
        ui->buttonGetProjectDiscovery->hide();
    }
    key = Config::generalConfig().value("riskiq_key").toString();
    if(!key.isEmpty()){
        ui->lineEditRiskIqKey->setText(key);
    }
    key = Config::generalConfig().value("riskiq_username").toString();
    if(!key.isEmpty()){
        ui->lineEditRiskIqUsername->setText(key);
        ui->buttonGetRiskIq->hide();
    }
    key = Config::generalConfig().value("securitytrails").toString();
    if(!key.isEmpty()){
        ui->lineEditSecurityTrails->setText(key);
        ui->buttonGetSecurityTrails->hide();
    }
    key = Config::generalConfig().value("shodan").toString();
    if(!key.isEmpty()){
        ui->lineEditShodan->setText(key);
        ui->buttonGetShodan->hide();
    }
    key = Config::generalConfig().value("spyse").toString();
    if(!key.isEmpty()){
        ui->lineEditSpyse->setText(key);
        ui->buttonGetSpyse->hide();
    }
    key = Config::generalConfig().value("virustotalapi").toString();
    if(!key.isEmpty()){
        ui->lineEditVirusTotal->setText(key);
        ui->buttonGetVirusTotal->hide();
    }
    key = Config::generalConfig().value("binaryedge").toString();
    if(!key.isEmpty()){
        ui->lineEditBinaryEdge->setText(key);
        ui->buttonGetBinaryEdge->hide();
    }
    key = Config::generalConfig().value("c99").toString();
    if(!key.isEmpty()){
        ui->lineEditC99->setText(key);
        ui->buttonGetC99->hide();
    }
    key = Config::generalConfig().value("ipinfo").toString();
    if(!key.isEmpty()){
        ui->lineEditIpInfo->setText(key);
        ui->buttonGetIpInfo->hide();
    }
    key = Config::generalConfig().value("threatbook").toString();
    if(!key.isEmpty()){
        ui->lineEditThreatBook->setText(key);
        ui->buttonGetThreatBook->hide();
    }
    key = Config::generalConfig().value("whoisxmlapi").toString();
    if(!key.isEmpty()){
        ui->lineEditWhoisXmlApi->setText(key);
        ui->buttonGetWhoisXmlApi->hide();
    }
    key = Config::generalConfig().value("zetalytics").toString();
    if(!key.isEmpty()){
        ui->lineEditZETAlytics->setText(key);
        ui->buttonGetZETAlytics->hide();
    }
    key = Config::generalConfig().value("zoomeye").toString();
    if(!key.isEmpty()){
        ui->lineEditZoomEye->setText(key);
        ui->buttonGetZoomEye->hide();
    }
    key = Config::generalConfig().value("ipapi").toString();
    if(!key.isEmpty()){
        ui->lineEditIpApi->setText(key);
        ui->buttonGetIpApi->hide();
    }
    key = Config::generalConfig().value("viewdns").toString();
    if(!key.isEmpty()){
        ui->lineEditViewDns->setText(key);
        ui->buttonGetViewDns->hide();
    }
    key = Config::generalConfig().value("hackertarget").toString();
    if(!key.isEmpty()){
        ui->lineEditHackerTarget->setText(key);
        ui->buttonGetHackerTarget->hide();
    }
    key = Config::generalConfig().value("webresolver").toString();
    if(!key.isEmpty()){
        ui->lineEditWebResolver->setText(key);
        ui->buttonGetWebResolver->hide();
    }
    key = Config::generalConfig().value("circlUser").toString();
    if(!key.isEmpty()){
        ui->lineEditCirclUser->setText(key);
        /*
         */
    }
    key = Config::generalConfig().value("circlPwd").toString();
    if(!key.isEmpty()){
        ui->lineEditCirclPwd->setText(key);
        ui->buttonGetCircl->hide();
    }
    key = Config::generalConfig().value("mnemonic").toString();
    if(!key.isEmpty()){
        ui->lineEditMnemonic->setText(key);
        ui->buttonGetMnemonic->hide();
    }
    Config::generalConfig().endGroup();
}


void ApiKeysDialog::on_buttonGetCensys_clicked(){
    QDesktopServices::openUrl(QUrl("https://censys.io/login", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetVirusTotal_clicked(){
     QDesktopServices::openUrl(QUrl("https://www.virustotal.com/gui/join-us", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetGithub_clicked(){
    QDesktopServices::openUrl(QUrl("https://github.com/settings/tokens", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetShodan_clicked(){
    QDesktopServices::openUrl(QUrl("https://developer.shodan.io/api/requirements", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetCloudflare_clicked(){
    QDesktopServices::openUrl(QUrl("https://dash.cloudflare.com/profile/api-tokens", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetBing_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.microsoft.com/en-us/bing/apis/bing-web-search-api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetGoogle_clicked(){
    QDesktopServices::openUrl(QUrl("https://developers.google.com/maps/documentation/places/web-service/get-api-key", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetHunter_clicked(){
    QDesktopServices::openUrl(QUrl("https://hunter.io/api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetCertspotter_clicked(){
    QDesktopServices::openUrl(QUrl("https://sslmate.com/certspotter/api/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIntelX_clicked(){
    QDesktopServices::openUrl(QUrl("https://intelx.io/signup", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetSecurityTrails_clicked(){
    QDesktopServices::openUrl(QUrl("https://securitytrails.com/corp/api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetSpyse_clicked(){
    QDesktopServices::openUrl(QUrl("https://spyse.com/user/registration", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetRiskIq_clicked(){
    QDesktopServices::openUrl(QUrl("https://api.riskiq.net/api/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetPentestTools_clicked(){
    QDesktopServices::openUrl(QUrl("https://pentest-tools.com/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetProjectDiscovery_clicked(){
    QDesktopServices::openUrl(QUrl("https://chaos.projectdiscovery.io/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetBinaryEdge_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.binaryedge.io/pricing.html", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetC99_clicked(){
    QDesktopServices::openUrl(QUrl("http://api.c99.nl/dashboard/shop", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIpInfo_clicked(){
    QDesktopServices::openUrl(QUrl("https://ipinfo.io/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetThreatBook_clicked(){
    QDesktopServices::openUrl(QUrl("https://x.threatbook.cn/api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetWhoisXmlApi_clicked(){
    QDesktopServices::openUrl(QUrl("https://main.whoisxmlapi.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetZETAlytics_clicked(){
    QDesktopServices::openUrl(QUrl("https://zetalytics.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetZoomEye_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.zoomeye.org/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIpApi_clicked(){
    QDesktopServices::openUrl(QUrl("https://ipapi.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetViewDns_clicked(){
    QDesktopServices::openUrl(QUrl("https://viewdns.info/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetHackerTarget_clicked(){
    QDesktopServices::openUrl(QUrl("https://hackertarget.com/ip-tools/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetWebResolver_clicked(){
    QDesktopServices::openUrl(QUrl("https://webresolver.nl/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetCircl_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.circl.lu/services/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetMnemonic_clicked(){
    QDesktopServices::openUrl(QUrl("https://mnemonic.no/", QUrl::TolerantMode));
}
