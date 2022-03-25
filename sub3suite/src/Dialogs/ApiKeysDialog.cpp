/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "ApiKeysDialog.h"
#include "ui_ApiKeysDialog.h"

#include "src/utils/Config.h"
#include "src/modules/passive/OsintDefinitions.h"

#include <QUrl>
#include <QDesktopServices>


ApiKeysDialog::ApiKeysDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::ApiKeysDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/key.png"));

    this->m_loadApiKeys();
}
ApiKeysDialog::~ApiKeysDialog(){
    delete ui;
}

void ApiKeysDialog::on_buttonSave_clicked(){
    APIKEY.setValue("bing", ui->lineEditBing->text());
    APIKEY.setValue("censys_id", ui->lineEditCensysId->text());
    APIKEY.setValue("censys_secret", ui->lineEditCensysSecret->text());
    APIKEY.setValue(OSINT_MODULE_CERTSPOTTER, ui->lineEditCertspotter->text());
    APIKEY.setValue("cloudflare", ui->lineEditCloudflare->text());
    APIKEY.setValue(OSINT_MODULE_GITHUB, ui->lineEditGithub->text());
    APIKEY.setValue("google", ui->lineEditGoogle->text());
    APIKEY.setValue("hunter", ui->lineEditHunter->text());
    APIKEY.setValue("intelx", ui->lineEditIntelX->text());
    APIKEY.setValue("pentesttools", ui->lineEditPentestTools->text());
    APIKEY.setValue(OSINT_MODULE_PROJECTDISCOVERY, ui->lineEditProjectDiscovery->text());
    APIKEY.setValue("riskiq_key", ui->lineEditRiskIqKey->text());
    APIKEY.setValue("riskiq_secret", ui->lineEditRiskIqSecret->text());
    APIKEY.setValue("passivetotal_key", ui->lineEditPassiveTotalKey->text());
    APIKEY.setValue("passivetotal_username", ui->lineEditPassiveTotalUsername->text());
    APIKEY.setValue(OSINT_MODULE_SECURITYTRAILS, ui->lineEditSecurityTrails->text());
    APIKEY.setValue(OSINT_MODULE_SHODAN, ui->lineEditShodan->text());
    APIKEY.setValue("spyse", ui->lineEditSpyse->text());
    APIKEY.setValue("virustotal", ui->lineEditVirusTotal->text());
    APIKEY.setValue(OSINT_MODULE_BINARYEDGE, ui->lineEditBinaryEdge->text());
    APIKEY.setValue(OSINT_MODULE_C99, ui->lineEditC99->text());
    APIKEY.setValue("ipapi", ui->lineEditIpApi->text());
    APIKEY.setValue("ipinfo", ui->lineEditIpInfo->text());
    APIKEY.setValue(OSINT_MODULE_THREATBOOK, ui->lineEditThreatBook->text());
    APIKEY.setValue(OSINT_MODULE_WHOISXMLAPI, ui->lineEditWhoisXmlApi->text());
    APIKEY.setValue(OSINT_MODULE_ZOOMEYE, ui->lineEditZoomEye->text());
    APIKEY.setValue(OSINT_MODULE_VIEWDNS, ui->lineEditViewDns->text());
    APIKEY.setValue(OSINT_MODULE_HACKERTARGET, ui->lineEditHackerTarget->text());
    APIKEY.setValue(OSINT_MODULE_WEBRESOLVER, ui->lineEditWebResolver->text());
    APIKEY.setValue("circlUser", ui->lineEditCirclUser->text());
    APIKEY.setValue("circlPwd", ui->lineEditCirclPwd->text());
    APIKEY.setValue(OSINT_MODULE_MNEMONIC, ui->lineEditMnemonic->text());
    APIKEY.setValue(OSINT_MODULE_ROBTEX, ui->lineEditRobtex->text());
    APIKEY.setValue(OSINT_MODULE_OTX, ui->lineEditRobtex->text());
    APIKEY.setValue(OSINT_MODULE_BUILTWITH, ui->lineEditBuiltWith->text());
    APIKEY.setValue(OSINT_MODULE_DNSLYTICS, ui->lineEditDnslytics->text());
    APIKEY.setValue("domaintools_username", ui->lineEditDomainToolsUsername->text());
    APIKEY.setValue("domaintools_key", ui->lineEditDomainToolsKey->text());
    APIKEY.setValue(OSINT_MODULE_ONYPHE, ui->lineEditOnyphe->text());
    APIKEY.setValue(OSINT_MODULE_FULLHUNT, ui->lineEditFullHunt->text());
    APIKEY.setValue(OSINT_MODULE_SPAMHAUS, ui->lineEditSpamHaus->text());
    APIKEY.setValue("debounce", ui->lineEditDebounce->text());
    APIKEY.setValue(OSINT_MODULE_HAVEIBEENPAWNED, ui->lineEditHaveIBeenPawned->text());
    APIKEY.setValue(OSINT_MODULE_HOST, ui->lineEditHost->text());
    APIKEY.setValue(OSINT_MODULE_JSONWHOIS, ui->lineEditJsonWhois->text());
    APIKEY.setValue(OSINT_MODULE_LEAKIX, ui->lineEditLeakIX->text());
    APIKEY.setValue(OSINT_MODULE_NETWORKSDB, ui->lineEditNetworksDB->text());
    APIKEY.setValue("neutrinoapi_key", ui->lineEditNeutrinoApiKey->text());
    APIKEY.setValue("neutrinoapi_uid", ui->lineEditNeutrinoApiUID->text());
    APIKEY.setValue(OSINT_MODULE_SEON, ui->lineEditSeon->text());
    APIKEY.setValue(OSINT_MODULE_SPYONWEB, ui->lineEditSpyOnWeb->text());
    APIKEY.setValue(OSINT_MODULE_WHATCMS, ui->lineEditWhatcms->text());
    APIKEY.setValue(OSINT_MODULE_WHOXY, ui->lineEditWhoxy->text());
    APIKEY.setValue("emailcrawlr", ui->lineEditEmailCrawlr->text());
    APIKEY.setValue("emailformat", ui->lineEditEmailFormat->text());
    APIKEY.setValue("emailrep", ui->lineEditEmailRep->text());
    APIKEY.setValue("snov", ui->lineEditSnov->text());
    APIKEY.setValue("ipdata", ui->lineEditIpData->text());
    APIKEY.setValue("ipgeolocation", ui->lineEditIpGeoLocation->text());
    APIKEY.setValue("ipregistry", ui->lineEditIpRegistry->text());
    APIKEY.setValue("ipstack", ui->lineEditIpStack->text());
    APIKEY.setValue("abuseipdb", ui->lineEditAbuseIPDB->text());
    APIKEY.setValue("fraudguard_user", ui->lineEditFraudGuardUser->text());
    APIKEY.setValue("fraudguard_pass", ui->lineEditFraudGuardPass->text());
    APIKEY.setValue("hybridanalysis", ui->lineEditHybridAnalysis->text());
    APIKEY.setValue("ipqualityscore", ui->lineEditIpQualityScore->text());
    APIKEY.setValue("leaklookup", ui->lineEditLeakLookup->text());
    APIKEY.setValue(OSINT_MODULE_ZETALYTICS, ui->lineEditZETAlytics->text());
    APIKEY.setValue(OSINT_MODULE_WAPPALYZER, ui->lineEditWappalyzer->text());
    accept();
}

void ApiKeysDialog::on_buttonCancel_clicked(){
    accept();
}

void ApiKeysDialog::m_loadApiKeys(){
    QString key;

    /* loading the keys.... */
    key = APIKEY.value("bing").toString();
    if(!key.isEmpty()){
        ui->lineEditBing->setText(key);
        ui->buttonGetBing->hide();
    }
    key = APIKEY.value("censys_id").toString();
    if(!key.isEmpty()){
        ui->lineEditCensysId->setText(key);
        ui->buttonGetCensys->hide();
    }
    key = APIKEY.value("censys_secret").toString();
    if(!key.isEmpty()){
        ui->lineEditCensysSecret->setText(key);
        ui->buttonGetCensys->hide();
    }
    key = APIKEY.value(OSINT_MODULE_CERTSPOTTER).toString();
    if(!key.isEmpty()){
        ui->lineEditCertspotter->setText(key);
        ui->buttonGetCertspotter->hide();
    }
    key = APIKEY.value("cloudflare").toString();
    if(!key.isEmpty()){
        ui->lineEditCloudflare->setText(key);
        ui->buttonGetCloudflare->hide();
    }
    key = APIKEY.value(OSINT_MODULE_GITHUB).toString();
    if(!key.isEmpty()){
        ui->lineEditGithub->setText(key);
        ui->buttonGetGithub->hide();
    }
    key = APIKEY.value("google").toString();
    if(!key.isEmpty()){
        ui->lineEditGoogle->setText(key);
        ui->buttonGetGoogle->hide();
    }
    key = APIKEY.value("hunter").toString();
    if(!key.isEmpty()){
        ui->lineEditHunter->setText(key);
        ui->buttonGetHunter->hide();
    }
    key = APIKEY.value("intelx").toString();
    if(!key.isEmpty()){
        ui->lineEditIntelX->setText(key);
        ui->buttonGetIntelX->hide();
    }
    key = APIKEY.value("pentesttools").toString();
    if(!key.isEmpty()){
        ui->lineEditPentestTools->setText(key);
        ui->buttonGetPentestTools->hide();
    }
    key = APIKEY.value(OSINT_MODULE_PROJECTDISCOVERY).toString();
    if(!key.isEmpty()){
        ui->lineEditProjectDiscovery->setText(key);
        ui->buttonGetProjectDiscovery->hide();
    }
    key = APIKEY.value("riskiq_key").toString();
    if(!key.isEmpty()){
        ui->lineEditRiskIqKey->setText(key);
    }
    key = APIKEY.value("riskiq_secret").toString();
    if(!key.isEmpty()){
        ui->lineEditRiskIqSecret->setText(key);
        ui->buttonGetRiskIq->hide();
    }
    key = APIKEY.value("passivetotal_key").toString();
    if(!key.isEmpty()){
        ui->lineEditPassiveTotalKey->setText(key);
    }
    key = APIKEY.value("passivetotal_username").toString();
    if(!key.isEmpty()){
        ui->lineEditPassiveTotalUsername->setText(key);
        ui->buttonGetPassiveTotal->hide();
    }
    key = APIKEY.value(OSINT_MODULE_SECURITYTRAILS).toString();
    if(!key.isEmpty()){
        ui->lineEditSecurityTrails->setText(key);
        ui->buttonGetSecurityTrails->hide();
    }
    key = APIKEY.value(OSINT_MODULE_SHODAN).toString();
    if(!key.isEmpty()){
        ui->lineEditShodan->setText(key);
        ui->buttonGetShodan->hide();
    }
    key = APIKEY.value("spyse").toString();
    if(!key.isEmpty()){
        ui->lineEditSpyse->setText(key);
        ui->buttonGetSpyse->hide();
    }
    key = APIKEY.value("virustotal").toString();
    if(!key.isEmpty()){
        ui->lineEditVirusTotal->setText(key);
        ui->buttonGetVirusTotal->hide();
    }
    key = APIKEY.value(OSINT_MODULE_BINARYEDGE).toString();
    if(!key.isEmpty()){
        ui->lineEditBinaryEdge->setText(key);
        ui->buttonGetBinaryEdge->hide();
    }
    key = APIKEY.value(OSINT_MODULE_C99).toString();
    if(!key.isEmpty()){
        ui->lineEditC99->setText(key);
        ui->buttonGetC99->hide();
    }
    key = APIKEY.value("ipinfo").toString();
    if(!key.isEmpty()){
        ui->lineEditIpInfo->setText(key);
        ui->buttonGetIpInfo->hide();
    }
    key = APIKEY.value(OSINT_MODULE_THREATBOOK).toString();
    if(!key.isEmpty()){
        ui->lineEditThreatBook->setText(key);
        ui->buttonGetThreatBook->hide();
    }
    key = APIKEY.value(OSINT_MODULE_WHOISXMLAPI).toString();
    if(!key.isEmpty()){
        ui->lineEditWhoisXmlApi->setText(key);
        ui->buttonGetWhoisXmlApi->hide();
    }
    key = APIKEY.value(OSINT_MODULE_ZETALYTICS).toString();
    if(!key.isEmpty()){
        ui->lineEditZETAlytics->setText(key);
        ui->buttonGetZETAlytics->hide();
    }
    key = APIKEY.value(OSINT_MODULE_ZOOMEYE).toString();
    if(!key.isEmpty()){
        ui->lineEditZoomEye->setText(key);
        ui->buttonGetZoomEye->hide();
    }
    key = APIKEY.value("ipapi").toString();
    if(!key.isEmpty()){
        ui->lineEditIpApi->setText(key);
        ui->buttonGetIpApi->hide();
    }
    key = APIKEY.value(OSINT_MODULE_VIEWDNS).toString();
    if(!key.isEmpty()){
        ui->lineEditViewDns->setText(key);
        ui->buttonGetViewDns->hide();
    }
    key = APIKEY.value(OSINT_MODULE_HACKERTARGET).toString();
    if(!key.isEmpty()){
        ui->lineEditHackerTarget->setText(key);
        ui->buttonGetHackerTarget->hide();
    }
    key = APIKEY.value(OSINT_MODULE_WEBRESOLVER).toString();
    if(!key.isEmpty()){
        ui->lineEditWebResolver->setText(key);
        ui->buttonGetWebResolver->hide();
    }
    key = APIKEY.value("circlUser").toString();
    if(!key.isEmpty()){
        ui->lineEditCirclUser->setText(key);
        /*
         */
    }
    key = APIKEY.value("circlPwd").toString();
    if(!key.isEmpty()){
        ui->lineEditCirclPwd->setText(key);
        ui->buttonGetCircl->hide();
    }
    key = APIKEY.value(OSINT_MODULE_MNEMONIC).toString();
    if(!key.isEmpty()){
        ui->lineEditMnemonic->setText(key);
        ui->buttonGetMnemonic->hide();
    }
    key = APIKEY.value(OSINT_MODULE_ROBTEX).toString();
    if(!key.isEmpty()){
        ui->lineEditRobtex->setText(key);
        ui->buttonGetRobtex->hide();
    }
    key = APIKEY.value(OSINT_MODULE_OTX).toString();
    if(!key.isEmpty()){
        ui->lineEditOtx->setText(key);
        ui->buttonGetOtx->hide();
    }
    key = APIKEY.value(OSINT_MODULE_BUILTWITH).toString();
    if(!key.isEmpty()){
        ui->lineEditBuiltWith->setText(key);
        ui->buttonGetBuiltWith->hide();
    }
    key = APIKEY.value(OSINT_MODULE_DNSLYTICS).toString();
    if(!key.isEmpty()){
        ui->lineEditDnslytics->setText(key);
        ui->buttonGetDnslytics->hide();
    }
    key = APIKEY.value("domaintools_username").toString();
    if(!key.isEmpty()){
        ui->lineEditDomainToolsUsername->setText(key);
        ui->buttonGetDomainTools->hide();
    }
    key = APIKEY.value("domaintools_key").toString();
    if(!key.isEmpty()){
        ui->lineEditDomainToolsKey->setText(key);
        ui->buttonGetDomainTools->hide();
    }
    key = APIKEY.value(OSINT_MODULE_ONYPHE).toString();
    if(!key.isEmpty()){
        ui->lineEditOnyphe->setText(key);
        ui->buttonGetOnyphe->hide();
    }
    key = APIKEY.value(OSINT_MODULE_FULLHUNT).toString();
    if(!key.isEmpty()){
        ui->lineEditFullHunt->setText(key);
        ui->buttonGetFullHunt->hide();
    }
    key = APIKEY.value(OSINT_MODULE_SPAMHAUS).toString();
    if(!key.isEmpty()){
        ui->lineEditSpamHaus->setText(key);
        ui->buttonGetSpamHaus->hide();
    }
    key = APIKEY.value("debounce").toString();
    if(!key.isEmpty()){
        ui->lineEditDebounce->setText(key);
        ui->buttonGetDebounce->hide();
    }
    key = APIKEY.value(OSINT_MODULE_HAVEIBEENPAWNED).toString();
    if(!key.isEmpty()){
        ui->lineEditHaveIBeenPawned->setText(key);
        ui->buttonGetHaveIBeenPawned->hide();
    }
    key = APIKEY.value(OSINT_MODULE_HOST).toString();
    if(!key.isEmpty()){
        ui->lineEditHost->setText(key);
        ui->buttonGetHost->hide();
    }
    key = APIKEY.value(OSINT_MODULE_JSONWHOIS).toString();
    if(!key.isEmpty()){
        ui->lineEditJsonWhois->setText(key);
        ui->buttonGetJsonWhois->hide();
    }
    key = APIKEY.value(OSINT_MODULE_LEAKIX).toString();
    if(!key.isEmpty()){
        ui->lineEditLeakIX->setText(key);
        ui->buttonGetLeakIX->hide();
    }
    key = APIKEY.value(OSINT_MODULE_NETWORKSDB).toString();
    if(!key.isEmpty()){
        ui->lineEditNetworksDB->setText(key);
        ui->buttonGetNetworksDB->hide();
    }
    //...
    key = APIKEY.value("neutrinoapi_key").toString();
    QString uid = APIKEY.value("neutrinoapi_uid").toString();
    if(!key.isEmpty() && !uid.isEmpty()){
        ui->lineEditNeutrinoApiKey->setText(key);
        ui->lineEditNeutrinoApiUID->setText(uid);
        ui->buttonGetNeutrinoApi->hide();
    }
    //...
    key = APIKEY.value(OSINT_MODULE_SEON).toString();
    if(!key.isEmpty()){
        ui->lineEditSeon->setText(key);
        ui->buttonGetSeon->hide();
    }
    key = APIKEY.value(OSINT_MODULE_SPYONWEB).toString();
    if(!key.isEmpty()){
        ui->lineEditSpyOnWeb->setText(key);
        ui->buttonGetSpyOnWeb->hide();
    }
    key = APIKEY.value(OSINT_MODULE_WHATCMS).toString();
    if(!key.isEmpty()){
        ui->lineEditWhatcms->setText(key);
        ui->buttonGetWhatcms->hide();
    }
    key = APIKEY.value(OSINT_MODULE_WHOXY).toString();
    if(!key.isEmpty()){
        ui->lineEditWhoxy->setText(key);
        ui->buttonGetWhoxy->hide();
    }
    key = APIKEY.value("emailcrawlr").toString();
    if(!key.isEmpty()){
        ui->lineEditEmailCrawlr->setText(key);
        ui->buttonGetEmailCrawlr->hide();
    }
    key = APIKEY.value("emailformat").toString();
    if(!key.isEmpty()){
        ui->lineEditEmailFormat->setText(key);
        ui->buttonGetEmailFormat->hide();
    }
    key = APIKEY.value("emailrep").toString();
    if(!key.isEmpty()){
        ui->lineEditEmailRep->setText(key);
        ui->buttonGetEmailRep->hide();
    }
    key = APIKEY.value("snov").toString();
    if(!key.isEmpty()){
        ui->lineEditSnov->setText(key);
        ui->buttonGetSnov->hide();
    }
    key = APIKEY.value("ipdata").toString();
    if(!key.isEmpty()){
        ui->lineEditIpData->setText(key);
        ui->buttonGetIpData->hide();
    }
    key = APIKEY.value("ipgeolocation").toString();
    if(!key.isEmpty()){
        ui->lineEditIpGeoLocation->setText(key);
        ui->buttonGetIpGeoLocation->hide();
    }
    key = APIKEY.value("ipregistry").toString();
    if(!key.isEmpty()){
        ui->lineEditIpRegistry->setText(key);
        ui->buttonGetIpRegistry->hide();
    }
    key = APIKEY.value("ipstack").toString();
    if(!key.isEmpty()){
        ui->lineEditIpStack->setText(key);
        ui->buttonGetIpStack->hide();
    }
    key = APIKEY.value("abuseipdb").toString();
    if(!key.isEmpty()){
        ui->lineEditAbuseIPDB->setText(key);
        ui->buttonGetAbuseIPDB->hide();
    }
    //...
    QString user = APIKEY.value("fraudguard_user").toString();
    QString pass = APIKEY.value("fraudguard_pass").toString();
    if(!key.isEmpty() && !uid.isEmpty()){
        ui->lineEditFraudGuardUser->setText(user);
        ui->lineEditFraudGuardPass->setText(pass);
        ui->buttonGetFraudGuard->hide();
    }
    //...
    key = APIKEY.value("hybridanalysis").toString();
    if(!key.isEmpty()){
        ui->lineEditHybridAnalysis->setText(key);
        ui->buttonGetHybridAnalysis->hide();
    }
    key = APIKEY.value("ipqualityscore").toString();
    if(!key.isEmpty()){
        ui->lineEditIpQualityScore->setText(key);
        ui->buttonGetIpQualityScore->hide();
    }
    key = APIKEY.value("leaklookup").toString();
    if(!key.isEmpty()){
        ui->lineEditLeakLookup->setText(key);
        ui->buttonGetLeakLookup->hide();
    }
    key = APIKEY.value(OSINT_MODULE_WAPPALYZER).toString();
    if(!key.isEmpty()){
        ui->lineEditWappalyzer->setText(key);
        ui->buttonGetWappalyzer->hide();
    }
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

void ApiKeysDialog::on_buttonGetRobtex_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.robtex.com/api/#pro_api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetOtx_clicked(){
    QDesktopServices::openUrl(QUrl("https://otx.alienvault.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetBuiltWith_clicked(){
    QDesktopServices::openUrl(QUrl("https://api.builtwith.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetDnslytics_clicked(){
    QDesktopServices::openUrl(QUrl("https://dnslytics.com/api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetDomainTools_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.domaintools.com/resources/api-documentation/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetOnyphe_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.onyphe.io/pricing/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetFullHunt_clicked(){
    QDesktopServices::openUrl(QUrl("https://fullhunt.io/pricing/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetSpamHaus_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.spamhaus.com/developer/sia/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetDebounce_clicked(){
    QDesktopServices::openUrl(QUrl("https://app.debounce.io/login", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetHaveIBeenPawned_clicked(){
    QDesktopServices::openUrl(QUrl("https://haveibeenpwned.com/API/Key", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetHost_clicked(){
    QDesktopServices::openUrl(QUrl("https://host.io/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetJsonWhois_clicked(){
    QDesktopServices::openUrl(QUrl("https://jsonwhois.com/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetLeakIX_clicked(){
    QDesktopServices::openUrl(QUrl("https://leakix.net/auth/login", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetNetworksDB_clicked(){
    QDesktopServices::openUrl(QUrl("https://networksdb.io/api/plans", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetNeutrinoApi_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.neutrinoapi.com/plans/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetSeon_clicked(){
    QDesktopServices::openUrl(QUrl("https://seon.io/pricing/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetSpyOnWeb_clicked(){
    QDesktopServices::openUrl(QUrl("https://api.spyonweb.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetWhatcms_clicked(){
    QDesktopServices::openUrl(QUrl("https://whatcms.org/API", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetWhoxy_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.whoxy.com/", QUrl::TolerantMode));
}
///
/// use referral code...
/// https://emailcrawlr.com/?ref_code=8d7e7b98
///
void ApiKeysDialog::on_buttonGetEmailCrawlr_clicked(){
    QDesktopServices::openUrl(QUrl("https://emailcrawlr.com/#pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetEmailFormat_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.email-format.com/i/pricing/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetEmailRep_clicked(){
    QDesktopServices::openUrl(QUrl("https://emailrep.io/key", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetSnov_clicked(){
    QDesktopServices::openUrl(QUrl("https://snov.io/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIpData_clicked(){
    QDesktopServices::openUrl(QUrl("https://ipdata.co/pricing.html", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIpGeoLocation_clicked(){
    QDesktopServices::openUrl(QUrl("https://ipgeolocation.io/pricing.html", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIpRegistry_clicked(){
    QDesktopServices::openUrl(QUrl("https://ipregistry.co/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIpStack_clicked(){
    QDesktopServices::openUrl(QUrl("https://ipstack.com/product", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetAbuseIPDB_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.abuseipdb.com/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetFraudGuard_clicked(){
    QDesktopServices::openUrl(QUrl("https://fraudguard.io/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetHybridAnalysis_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.hybrid-analysis.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetIpQualityScore_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.ipqualityscore.com/create-account", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetLeakLookup_clicked(){
    QDesktopServices::openUrl(QUrl("https://leak-lookup.com/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetWappalyzer_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.wappalyzer.com/pricing/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetPassiveTotal_clicked(){
    QDesktopServices::openUrl(QUrl("https://community.riskiq.com/", QUrl::TolerantMode));
}
