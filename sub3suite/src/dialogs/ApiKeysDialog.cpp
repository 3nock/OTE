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
    gConfig.keys.Censys.id = ui->lineEditCensysId->text();
    gConfig.keys.Censys.secret = ui->lineEditCensysSecret->text();
    gConfig.keys.CertSpotter = ui->lineEditCertspotter->text();
    gConfig.keys.Github = ui->lineEditGithub->text();
    gConfig.keys.Hunter = ui->lineEditHunter->text();
    gConfig.keys.ProjectDiscovery = ui->lineEditProjectDiscovery->text();
    gConfig.keys.RiskIQ.key = ui->lineEditRiskIqKey->text();
    gConfig.keys.RiskIQ.secret = ui->lineEditRiskIqSecret->text();
    gConfig.keys.PassiveTotal.key = ui->lineEditPassiveTotalKey->text();
    gConfig.keys.PassiveTotal.username = ui->lineEditPassiveTotalUsername->text();
    gConfig.keys.SecurityTrails = ui->lineEditSecurityTrails->text();
    gConfig.keys.Shodan = ui->lineEditShodan->text();
    gConfig.keys.Spyse = ui->lineEditSpyse->text();
    gConfig.keys.VirusTotal = ui->lineEditVirusTotal->text();
    gConfig.keys.BinaryEdge = ui->lineEditBinaryEdge->text();
    gConfig.keys.C99 = ui->lineEditC99->text();
    gConfig.keys.IpApi = ui->lineEditIpApi->text();
    gConfig.keys.IpInfo = ui->lineEditIpInfo->text();
    gConfig.keys.ThreatBook = ui->lineEditThreatBook->text();
    gConfig.keys.WhoIsXMLAPI = ui->lineEditWhoisXmlApi->text();
    gConfig.keys.ZoomEye = ui->lineEditZoomEye->text();
    gConfig.keys.ViewDns = ui->lineEditViewDns->text();
    gConfig.keys.HackerTarget = ui->lineEditHackerTarget->text();
    gConfig.keys.WebResolver = ui->lineEditWebResolver->text();
    gConfig.keys.Circl.user = ui->lineEditCirclUser->text();
    gConfig.keys.Circl.pwd = ui->lineEditCirclPwd->text();
    gConfig.keys.Mnemonic = ui->lineEditMnemonic->text();
    gConfig.keys.Robtex = ui->lineEditRobtex->text();
    gConfig.keys.Otx = ui->lineEditRobtex->text();
    gConfig.keys.BuiltWith = ui->lineEditBuiltWith->text();
    gConfig.keys.DNSlytics = ui->lineEditDnslytics->text();
    gConfig.keys.DomainTools.username = ui->lineEditDomainToolsUsername->text();
    gConfig.keys.DomainTools.key = ui->lineEditDomainToolsKey->text();
    gConfig.keys.Onyphe = ui->lineEditOnyphe->text();
    gConfig.keys.FullHunt = ui->lineEditFullHunt->text();
    gConfig.keys.SpamHaus = ui->lineEditSpamHaus->text();
    gConfig.keys.Debounce = ui->lineEditDebounce->text();
    gConfig.keys.HaveIBeenPawned = ui->lineEditHaveIBeenPawned->text();
    gConfig.keys.Host = ui->lineEditHost->text();
    gConfig.keys.JsonWhois = ui->lineEditJsonWhois->text();
    gConfig.keys.LeakIX = ui->lineEditLeakIX->text();
    gConfig.keys.NetworksDB = ui->lineEditNetworksDB->text();
    gConfig.keys.neutrinoapi.key = ui->lineEditNeutrinoApiKey->text();
    gConfig.keys.neutrinoapi.uid = ui->lineEditNeutrinoApiUID->text();
    gConfig.keys.Seon = ui->lineEditSeon->text();
    gConfig.keys.SpyOnWeb = ui->lineEditSpyOnWeb->text();
    gConfig.keys.WhatCMS = ui->lineEditWhatcms->text();
    gConfig.keys.Whoxy = ui->lineEditWhoxy->text();
    gConfig.keys.EmailCrawlr = ui->lineEditEmailCrawlr->text();
    gConfig.keys.EmailFormat = ui->lineEditEmailFormat->text();
    gConfig.keys.EmailRep = ui->lineEditEmailRep->text();
    gConfig.keys.Snov = ui->lineEditSnov->text();
    gConfig.keys.IpData = ui->lineEditIpData->text();
    gConfig.keys.IpGeoLocation = ui->lineEditIpGeoLocation->text();
    gConfig.keys.IpRegistry = ui->lineEditIpRegistry->text();
    gConfig.keys.IpStack = ui->lineEditIpStack->text();
    gConfig.keys.AbuseIPDB = ui->lineEditAbuseIPDB->text();
    gConfig.keys.fraudguard.user = ui->lineEditFraudGuardUser->text();
    gConfig.keys.fraudguard.pass = ui->lineEditFraudGuardPass->text();
    gConfig.keys.HybridAnalysis = ui->lineEditHybridAnalysis->text();
    gConfig.keys.IpQualityScore = ui->lineEditIpQualityScore->text();
    gConfig.keys.LeakLookup = ui->lineEditLeakLookup->text();
    gConfig.keys.ZETAlytics = ui->lineEditZETAlytics->text();
    gConfig.keys.Wappalyzer = ui->lineEditWappalyzer->text();
    gConfig.keys.Ipfy = ui->lineEditIpfy->text();
    accept();
}

void ApiKeysDialog::on_buttonCancel_clicked(){
    accept();
}

void ApiKeysDialog::m_loadApiKeys(){
    QString key;

    /* loading the keys.... */
    key = gConfig.keys.Censys.id;
    if(!key.isEmpty()){
        ui->lineEditCensysId->setText(key);
        ui->buttonGetCensys->hide();
    }
    key = gConfig.keys.Censys.secret;
    if(!key.isEmpty()){
        ui->lineEditCensysSecret->setText(key);
        ui->buttonGetCensys->hide();
    }
    key = gConfig.keys.CertSpotter;
    if(!key.isEmpty()){
        ui->lineEditCertspotter->setText(key);
        ui->buttonGetCertspotter->hide();
    }
    key = gConfig.keys.Github;
    if(!key.isEmpty()){
        ui->lineEditGithub->setText(key);
        ui->buttonGetGithub->hide();
    }
    key = gConfig.keys.Hunter;
    if(!key.isEmpty()){
        ui->lineEditHunter->setText(key);
        ui->buttonGetHunter->hide();
    }
    key = gConfig.keys.ProjectDiscovery;
    if(!key.isEmpty()){
        ui->lineEditProjectDiscovery->setText(key);
        ui->buttonGetProjectDiscovery->hide();
    }
    key = gConfig.keys.RiskIQ.key;
    if(!key.isEmpty()){
        ui->lineEditRiskIqKey->setText(key);
    }
    key = gConfig.keys.RiskIQ.secret;
    if(!key.isEmpty()){
        ui->lineEditRiskIqSecret->setText(key);
        ui->buttonGetRiskIq->hide();
    }
    key = gConfig.keys.PassiveTotal.key;
    if(!key.isEmpty()){
        ui->lineEditPassiveTotalKey->setText(key);
    }
    key = gConfig.keys.PassiveTotal.username;
    if(!key.isEmpty()){
        ui->lineEditPassiveTotalUsername->setText(key);
        ui->buttonGetPassiveTotal->hide();
    }
    key = gConfig.keys.SecurityTrails;
    if(!key.isEmpty()){
        ui->lineEditSecurityTrails->setText(key);
        ui->buttonGetSecurityTrails->hide();
    }
    key = gConfig.keys.Shodan;
    if(!key.isEmpty()){
        ui->lineEditShodan->setText(key);
        ui->buttonGetShodan->hide();
    }
    key = gConfig.keys.Spyse;
    if(!key.isEmpty()){
        ui->lineEditSpyse->setText(key);
        ui->buttonGetSpyse->hide();
    }
    key = gConfig.keys.VirusTotal;
    if(!key.isEmpty()){
        ui->lineEditVirusTotal->setText(key);
        ui->buttonGetVirusTotal->hide();
    }
    key = gConfig.keys.BinaryEdge;
    if(!key.isEmpty()){
        ui->lineEditBinaryEdge->setText(key);
        ui->buttonGetBinaryEdge->hide();
    }
    key = gConfig.keys.C99;
    if(!key.isEmpty()){
        ui->lineEditC99->setText(key);
        ui->buttonGetC99->hide();
    }
    key = gConfig.keys.IpInfo;
    if(!key.isEmpty()){
        ui->lineEditIpInfo->setText(key);
        ui->buttonGetIpInfo->hide();
    }
    key = gConfig.keys.ThreatBook;
    if(!key.isEmpty()){
        ui->lineEditThreatBook->setText(key);
        ui->buttonGetThreatBook->hide();
    }
    key = gConfig.keys.WhoIsXMLAPI;
    if(!key.isEmpty()){
        ui->lineEditWhoisXmlApi->setText(key);
        ui->buttonGetWhoisXmlApi->hide();
    }
    key = gConfig.keys.ZETAlytics;
    if(!key.isEmpty()){
        ui->lineEditZETAlytics->setText(key);
        ui->buttonGetZETAlytics->hide();
    }
    key = gConfig.keys.ZoomEye;
    if(!key.isEmpty()){
        ui->lineEditZoomEye->setText(key);
        ui->buttonGetZoomEye->hide();
    }
    key = gConfig.keys.IpApi;
    if(!key.isEmpty()){
        ui->lineEditIpApi->setText(key);
        ui->buttonGetIpApi->hide();
    }
    key = gConfig.keys.ViewDns;
    if(!key.isEmpty()){
        ui->lineEditViewDns->setText(key);
        ui->buttonGetViewDns->hide();
    }
    key = gConfig.keys.HackerTarget;
    if(!key.isEmpty()){
        ui->lineEditHackerTarget->setText(key);
        ui->buttonGetHackerTarget->hide();
    }
    key = gConfig.keys.WebResolver;
    if(!key.isEmpty()){
        ui->lineEditWebResolver->setText(key);
        ui->buttonGetWebResolver->hide();
    }
    key = gConfig.keys.Circl.user;
    if(!key.isEmpty()){
        ui->lineEditCirclUser->setText(key);
        /*
         */
    }
    key = gConfig.keys.Circl.pwd;
    if(!key.isEmpty()){
        ui->lineEditCirclPwd->setText(key);
        ui->buttonGetCircl->hide();
    }
    key = gConfig.keys.Mnemonic;
    if(!key.isEmpty()){
        ui->lineEditMnemonic->setText(key);
        ui->buttonGetMnemonic->hide();
    }
    key = gConfig.keys.Robtex;
    if(!key.isEmpty()){
        ui->lineEditRobtex->setText(key);
        ui->buttonGetRobtex->hide();
    }
    key = gConfig.keys.Otx;
    if(!key.isEmpty()){
        ui->lineEditOtx->setText(key);
        ui->buttonGetOtx->hide();
    }
    key = gConfig.keys.BuiltWith;
    if(!key.isEmpty()){
        ui->lineEditBuiltWith->setText(key);
        ui->buttonGetBuiltWith->hide();
    }
    key = gConfig.keys.DNSlytics;
    if(!key.isEmpty()){
        ui->lineEditDnslytics->setText(key);
        ui->buttonGetDnslytics->hide();
    }
    key = gConfig.keys.DomainTools.username;
    if(!key.isEmpty()){
        ui->lineEditDomainToolsUsername->setText(key);
        ui->buttonGetDomainTools->hide();
    }
    key = gConfig.keys.DomainTools.key;
    if(!key.isEmpty()){
        ui->lineEditDomainToolsKey->setText(key);
        ui->buttonGetDomainTools->hide();
    }
    key = gConfig.keys.Onyphe;
    if(!key.isEmpty()){
        ui->lineEditOnyphe->setText(key);
        ui->buttonGetOnyphe->hide();
    }
    key = gConfig.keys.FullHunt;
    if(!key.isEmpty()){
        ui->lineEditFullHunt->setText(key);
        ui->buttonGetFullHunt->hide();
    }
    key = gConfig.keys.SpamHaus;
    if(!key.isEmpty()){
        ui->lineEditSpamHaus->setText(key);
        ui->buttonGetSpamHaus->hide();
    }
    key = gConfig.keys.Debounce;
    if(!key.isEmpty()){
        ui->lineEditDebounce->setText(key);
        ui->buttonGetDebounce->hide();
    }
    key = gConfig.keys.HaveIBeenPawned;
    if(!key.isEmpty()){
        ui->lineEditHaveIBeenPawned->setText(key);
        ui->buttonGetHaveIBeenPawned->hide();
    }
    key = gConfig.keys.Host;
    if(!key.isEmpty()){
        ui->lineEditHost->setText(key);
        ui->buttonGetHost->hide();
    }
    key = gConfig.keys.JsonWhois;
    if(!key.isEmpty()){
        ui->lineEditJsonWhois->setText(key);
        ui->buttonGetJsonWhois->hide();
    }
    key = gConfig.keys.LeakIX;
    if(!key.isEmpty()){
        ui->lineEditLeakIX->setText(key);
        ui->buttonGetLeakIX->hide();
    }
    key = gConfig.keys.NetworksDB;
    if(!key.isEmpty()){
        ui->lineEditNetworksDB->setText(key);
        ui->buttonGetNetworksDB->hide();
    }
    //...
    key = gConfig.keys.neutrinoapi.key;
    QString uid = gConfig.keys.neutrinoapi.uid;
    if(!key.isEmpty() && !uid.isEmpty()){
        ui->lineEditNeutrinoApiKey->setText(key);
        ui->lineEditNeutrinoApiUID->setText(uid);
        ui->buttonGetNeutrinoApi->hide();
    }
    //...
    key = gConfig.keys.Seon;
    if(!key.isEmpty()){
        ui->lineEditSeon->setText(key);
        ui->buttonGetSeon->hide();
    }
    key = gConfig.keys.SpyOnWeb;
    if(!key.isEmpty()){
        ui->lineEditSpyOnWeb->setText(key);
        ui->buttonGetSpyOnWeb->hide();
    }
    key = gConfig.keys.WhatCMS;
    if(!key.isEmpty()){
        ui->lineEditWhatcms->setText(key);
        ui->buttonGetWhatcms->hide();
    }
    key = gConfig.keys.Whoxy;
    if(!key.isEmpty()){
        ui->lineEditWhoxy->setText(key);
        ui->buttonGetWhoxy->hide();
    }
    key = gConfig.keys.EmailCrawlr;
    if(!key.isEmpty()){
        ui->lineEditEmailCrawlr->setText(key);
        ui->buttonGetEmailCrawlr->hide();
    }
    key = gConfig.keys.EmailFormat;
    if(!key.isEmpty()){
        ui->lineEditEmailFormat->setText(key);
        ui->buttonGetEmailFormat->hide();
    }
    key = gConfig.keys.EmailRep;
    if(!key.isEmpty()){
        ui->lineEditEmailRep->setText(key);
        ui->buttonGetEmailRep->hide();
    }
    key = gConfig.keys.Snov;
    if(!key.isEmpty()){
        ui->lineEditSnov->setText(key);
        ui->buttonGetSnov->hide();
    }
    key = gConfig.keys.IpData;
    if(!key.isEmpty()){
        ui->lineEditIpData->setText(key);
        ui->buttonGetIpData->hide();
    }
    key = gConfig.keys.IpGeoLocation;
    if(!key.isEmpty()){
        ui->lineEditIpGeoLocation->setText(key);
        ui->buttonGetIpGeoLocation->hide();
    }
    key = gConfig.keys.IpRegistry;
    if(!key.isEmpty()){
        ui->lineEditIpRegistry->setText(key);
        ui->buttonGetIpRegistry->hide();
    }
    key = gConfig.keys.IpStack;
    if(!key.isEmpty()){
        ui->lineEditIpStack->setText(key);
        ui->buttonGetIpStack->hide();
    }
    key = gConfig.keys.AbuseIPDB;
    if(!key.isEmpty()){
        ui->lineEditAbuseIPDB->setText(key);
        ui->buttonGetAbuseIPDB->hide();
    }
    //...
    QString user = gConfig.keys.fraudguard.user;
    QString pass = gConfig.keys.fraudguard.pass;
    if(!key.isEmpty() && !uid.isEmpty()){
        ui->lineEditFraudGuardUser->setText(user);
        ui->lineEditFraudGuardPass->setText(pass);
        ui->buttonGetFraudGuard->hide();
    }
    //...
    key = gConfig.keys.HybridAnalysis;
    if(!key.isEmpty()){
        ui->lineEditHybridAnalysis->setText(key);
        ui->buttonGetHybridAnalysis->hide();
    }
    key = gConfig.keys.IpQualityScore;
    if(!key.isEmpty()){
        ui->lineEditIpQualityScore->setText(key);
        ui->buttonGetIpQualityScore->hide();
    }
    key = gConfig.keys.LeakLookup;
    if(!key.isEmpty()){
        ui->lineEditLeakLookup->setText(key);
        ui->buttonGetLeakLookup->hide();
    }
    key = gConfig.keys.Wappalyzer;
    if(!key.isEmpty()){
        ui->lineEditWappalyzer->setText(key);
        ui->buttonGetWappalyzer->hide();
    }
    key = gConfig.keys.Ipfy;
    if(!key.isEmpty()){
        ui->lineEditIpfy->setText(key);
        ui->buttonGetIpfy->hide();
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

void ApiKeysDialog::on_buttonGetHunter_clicked(){
    QDesktopServices::openUrl(QUrl("https://hunter.io/api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_buttonGetCertspotter_clicked(){
    QDesktopServices::openUrl(QUrl("https://sslmate.com/certspotter/api/", QUrl::TolerantMode));
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

void ApiKeysDialog::on_buttonGetIpfy_clicked(){
    QDesktopServices::openUrl(QUrl("https://geo.ipify.org/pricing", QUrl::TolerantMode));
}
