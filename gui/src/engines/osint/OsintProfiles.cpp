#include "Osint.h"
#include "ui_Osint.h"
//...
#include <QSettings>
#include "src/utils/utils.h"
#include "src/utils/Config.h"


void Osint::on_useProfiles_clicked(bool checked){
    if(checked){
        ui->frameProfiles->show();
    }else{
        ui->frameProfiles->hide();
    }
}

void Osint::on_buttonLoadProfile_clicked(){
    ///
    /// opening the file containing the profiles...
    ///
    QSettings settings(m_currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.beginGroup(ui->comboBoxProfiles->currentText());

/***************************************************************
                          API
****************************************************************/

    if(settings.value("anubis").toString() == TRUE){
        ui->moduleAnubis->setChecked(true);
    }else{
        ui->moduleAnubis->setChecked(false);
    }
    if(settings.value("bgpview").toString() == TRUE){
        ui->moduleBgpview->setChecked(true);
    }else{
        ui->moduleBgpview->setChecked(false);
    }
    if(settings.value("binaryedge").toString() == TRUE){
        ui->moduleBinaryEdge->setChecked(true);
    }else{
        ui->moduleBinaryEdge->setChecked(false);
    }
    if(settings.value("hackertargetfree").toString() == TRUE){
        ui->moduleHackerTargetFree->setChecked(true);
    }else{
        ui->moduleHackerTargetFree->setChecked(false);
    }
    if(settings.value("hackertargetpaid").toString() == TRUE){
        ui->moduleHackerTargetPaid->setChecked(true);
    }else{
        ui->moduleHackerTargetPaid->setChecked(false);
    }
    if(settings.value("circl").toString() == TRUE){
        ui->moduleCircl->setChecked(true);
    }else{
        ui->moduleCircl->setChecked(false);
    }
    if(settings.value("hunter").toString() == TRUE){
        ui->moduleHunter->setChecked(true);
    }else{
        ui->moduleHunter->setChecked(false);
    }
    if(settings.value("mnemonicfree").toString() == TRUE){
        ui->moduleMnemonicFree->setChecked(true);
    }else{
        ui->moduleMnemonicFree->setChecked(false);
    }
    if(settings.value("mnemonicpaid").toString() == TRUE){
        ui->moduleMnemonicPaid->setChecked(true);
    }else{
        ui->moduleMnemonicPaid->setChecked(false);
    }
    if(settings.value(OSINT_OMNISINT).toString() == TRUE){
        ui->moduleOmnisint->setChecked(true);
    }else{
        ui->moduleOmnisint->setChecked(false);
    }
    if(settings.value("otxfree").toString() == TRUE){
        ui->moduleOtxFree->setChecked(true);
    }else{
        ui->moduleOtxFree->setChecked(false);
    }
    if(settings.value("otxpaid").toString() == TRUE){
        ui->moduleOtxPaid->setChecked(true);
    }else{
        ui->moduleOtxPaid->setChecked(false);
    }
    if(settings.value(OSINT_PROJECTDISCOVERY).toString() == TRUE){
        ui->moduleProjectdiscovery->setChecked(true);
    }
    else{
        ui->moduleProjectdiscovery->setChecked(false);
    }
    if(settings.value("riskiq").toString() == TRUE){
        ui->moduleRiskIq->setChecked(true);
    }
    else{
        ui->moduleRiskIq->setChecked(false);
    }
    if(settings.value("robtexfree").toString() == TRUE){
        ui->moduleRobtexFree->setChecked(true);
    }
    else{
        ui->moduleRobtexFree->setChecked(false);
    }
    if(settings.value("robtexpaid").toString() == TRUE){
        ui->moduleRobtexPaid->setChecked(true);
    }
    else{
        ui->moduleRobtexPaid->setChecked(false);
    }
    if(settings.value(OSINT_SECURITYTRAILS).toString() == TRUE){
        ui->moduleSecuritytrails->setChecked(true);
    }else{
        ui->moduleSecuritytrails->setChecked(false);
    }
    if(settings.value(OSINT_SHODAN).toString() == TRUE){
        ui->moduleShodan->setChecked(true);
    }else{
        ui->moduleShodan->setChecked(false);
    }
    if(settings.value(OSINT_SPYSE).toString() == TRUE){
        ui->moduleSpyse->setChecked(true);
    }else{
        ui->moduleSpyse->setChecked(false);
    }
    if(settings.value("sublist3r").toString() == TRUE){
        ui->moduleSublist3r->setChecked(true);
    }else{
        ui->moduleSublist3r->setChecked(false);
    }
    if(settings.value("threatbook").toString() == TRUE){
        ui->moduleThreatBook->setChecked(true);
    }else{
        ui->moduleThreatBook->setChecked(false);
    }
    if(settings.value(OSINT_THREATCROWD).toString() == TRUE){
        ui->moduleThreatcrowd->setChecked(true);
    }else{
        ui->moduleThreatcrowd->setChecked(false);
    }
    if(settings.value(OSINT_THREATMINER).toString() == TRUE){
        ui->moduleThreatminer->setChecked(true);
    }else{
        ui->moduleThreatminer->setChecked(false);
    }
    if(settings.value("viewdns").toString() == TRUE){
        ui->moduleViewDns->setChecked(true);
    }else{
        ui->moduleViewDns->setChecked(false);
    }
    if(settings.value("virustotal").toString() == TRUE){
        ui->moduleVirusTotal->setChecked(true);
    }else{
        ui->moduleVirusTotal->setChecked(false);
    }
    if(settings.value("webresolver").toString() == TRUE){
        ui->moduleWebResolver->setChecked(true);
    }else{
        ui->moduleWebResolver->setChecked(false);
    }
    if(settings.value("whoisxmlapi").toString() == TRUE){
        ui->moduleWhoisXmlApi->setChecked(true);
    }else{
        ui->moduleWhoisXmlApi->setChecked(false);
    }
    if(settings.value("zetalytics").toString() == TRUE){
        ui->moduleZetalytics->setChecked(true);
    }else{
        ui->moduleZetalytics->setChecked(false);
    }
    if(settings.value("zoomeye").toString() == TRUE){
        ui->moduleZoomeye->setChecked(true);
    }else{
        ui->moduleZoomeye->setChecked(false);
    }
    if(settings.value("ipinfo").toString() == TRUE){
        ui->moduleIpInfo->setChecked(true);
    }else{
        ui->moduleIpInfo->setChecked(false);
    }
    if(settings.value("dnslytics").toString() == TRUE){
        ui->moduleDnslytics->setChecked(true);
    }else{
        ui->moduleDnslytics->setChecked(false);
    }
    if(settings.value("maltiversefree").toString() == TRUE){
        ui->moduleMaltiverseFree->setChecked(true);
    }else{
        ui->moduleMaltiverseFree->setChecked(false);
    }
    if(settings.value("n45ht").toString() == TRUE){
        ui->moduleN45HT->setChecked(true);
    }else{
        ui->moduleN45HT->setChecked(false);
    }
    if(settings.value("onyphe").toString() == TRUE){
        ui->moduleOnyphe->setChecked(true);
    }else{
        ui->moduleOnyphe->setChecked(false);
    }
    if(settings.value("ripe").toString() == TRUE){
        ui->moduleRipe->setChecked(true);
    }else{
        ui->moduleRipe->setChecked(false);
    }
    if(settings.value("fullhunt").toString() == TRUE){
        ui->moduleFullHunt->setChecked(true);
    }else{
        ui->moduleFullHunt->setChecked(false);
    }
    if(settings.value("networksdb").toString() == TRUE){
        ui->moduleNetworksDB->setChecked(true);
    }else{
        ui->moduleNetworksDB->setChecked(false);
    }
    if(settings.value("spyonweb").toString() == TRUE){
        ui->moduleSpyOnWeb->setChecked(true);
    }else{
        ui->moduleSpyOnWeb->setChecked(false);
    }

/***************************************************************
                          ARCHIVES
****************************************************************/
    if(settings.value("archiveit").toString() == TRUE){
        ui->moduleArchiveit->setChecked(true);
    }else{
        ui->moduleArchiveit->setChecked(false);
    }
    if(settings.value("archivetoday").toString() == TRUE){
        ui->moduleArchiveToday->setChecked(true);
    }else{
        ui->moduleArchiveToday->setChecked(false);
    }
    if(settings.value("arquivo").toString() == TRUE){
        ui->moduleArquivo->setChecked(true);
    }else{
        ui->moduleArquivo->setChecked(false);
    }
    if(settings.value("commoncrawl").toString() == TRUE){
        ui->moduleCommonCrawl->setChecked(true);
    }else{
        ui->moduleCommonCrawl->setChecked(false);
    }
    if(settings.value("ukwebarchive").toString() == TRUE){
        ui->moduleUKWebArchive->setChecked(true);
    }else{
        ui->moduleUKWebArchive->setChecked(false);
    }
    if(settings.value(OSINT_WAYBACKMACHINE).toString() == TRUE){
        ui->moduleWaybackmachine->setChecked(true);
    }else{
        ui->moduleWaybackmachine->setChecked(false);
    }

/***************************************************************
                           CERTS
****************************************************************/
    if(settings.value(OSINT_BING).toString() == TRUE){
        ui->moduleBing->setChecked(true);
    }else{
        ui->moduleBing->setChecked(false);
    }
    if(settings.value(OSINT_GITHUB).toString() == TRUE){
        ui->moduleGithub->setChecked(true);
    }else{
        ui->moduleGithub->setChecked(false);
    }
    if(settings.value(OSINT_CENSYS).toString() == TRUE){
        ui->moduleCensys->setChecked(true);
    }else{
        ui->moduleCensys->setChecked(false);
    }
    if(settings.value(OSINT_CLOUDFLARE).toString() == TRUE){
        ui->moduleCloudflare->setChecked(true);
    }else{
        ui->moduleCloudflare->setChecked(false);
    }
    if(settings.value(OSINT_INTELX).toString() == TRUE){
        ui->moduleIntelx->setChecked(true);
    }else{
        ui->moduleIntelx->setChecked(false);
    }
    if(settings.value(OSINT_GOOGLE).toString() == TRUE){
        ui->moduleGoogle->setChecked(true);
    }else{
        ui->moduleGoogle->setChecked(false);
    }
    if(settings.value(OSINT_CERTSPOTTER).toString() == TRUE){
        ui->moduleCertspotter->setChecked(true);
    }else{
        ui->moduleCertspotter->setChecked(false);
    }
    if(settings.value(OSINT_CRTSH).toString() == TRUE){
        ui->moduleCrtsh->setChecked(true);
    }else{
        ui->moduleCrtsh->setChecked(false);
    }
    if(settings.value(OSINT_DOGPILE).toString() == TRUE){
        ui->moduleDogpile->setChecked(true);
    }else{
        ui->moduleDogpile->setChecked(false);
    }
    if(settings.value(OSINT_DUCKDUCKGO).toString() == TRUE){
        ui->moduleDuckduckgo->setChecked(true);
    }else{
        ui->moduleDuckduckgo->setChecked(false);
    }
    if(settings.value(OSINT_EXALEAD).toString() == TRUE){
        ui->moduleExalead->setChecked(true);
    }else{
        ui->moduleExalead->setChecked(false);
    }
    if(settings.value(OSINT_NETCRAFT).toString() == TRUE){
        ui->moduleNetcraft->setChecked(true);
    }else{
        ui->moduleNetcraft->setChecked(false);
    }
    if(settings.value(OSINT_SUIP).toString() == TRUE){
        ui->moduleSuip->setChecked(true);
    }else{
        ui->moduleSuip->setChecked(false);
    }
    if(settings.value(OSINT_TRELLO).toString() == TRUE){
        ui->moduleTrello->setChecked(true);
    }else{
        ui->moduleTrello->setChecked(false);
    }
    if(settings.value(OSINT_DNSBUFFEROVERRUN).toString() == TRUE){
        ui->moduleDnsbufferoverrun->setChecked(true);
    }else{
        ui->moduleDnsbufferoverrun->setChecked(false);
    }
    if(settings.value(OSINT_PKEY).toString() == TRUE){
        ui->modulePkey->setChecked(true);
    }else{
        ui->modulePkey->setChecked(false);
    }
    if(settings.value(OSINT_ASK).toString() == TRUE){
        ui->moduleAsk->setChecked(true);
    }else{
        ui->moduleAsk->setChecked(false);
    }
    if(settings.value(OSINT_BAIDU).toString() == TRUE){
        ui->moduleBaidu->setChecked(true);
    }else{
        ui->moduleBaidu->setChecked(false);
    }
    if(settings.value(OSINT_DNSDUMPSTER).toString() == TRUE){
        ui->moduleDnsdumpster->setChecked(true);
    }else{
        ui->moduleDnsdumpster->setChecked(false);
    }
    if(settings.value(OSINT_YAHOO).toString() == TRUE){
        ui->moduleYahoo->setChecked(true);
    }else{
        ui->moduleYahoo->setChecked(false);
    }
    if(settings.value(OSINT_URLSCAN).toString() == TRUE){
        ui->moduleUrlscan->setChecked(true);
    }else{
        ui->moduleUrlscan->setChecked(false);
    }
    if(settings.value(OSINT_RAPIDDNS).toString() == TRUE){
        ui->moduleRapiddns->setChecked(true);
    }
    else{
        ui->moduleRapiddns->setChecked(false);
    }
    if(settings.value(OSINT_PENTESTTOOLS).toString() == TRUE){
        ui->modulePentesttools->setChecked(true);
    }else{
        ui->modulePentesttools->setChecked(false);
    }
    settings.endGroup();
}

void Osint::on_buttonCreateProfile_clicked(){
    ///
    /// checks...
    ///
    if(ui->lineEditNewProfile->text().isEmpty())
        return;
    ///
    /// appending profile name on comboBox and config file...
    ///
    QString profileName = ui->lineEditNewProfile->text();
    ui->comboBoxProfiles->addItem(profileName);
    ui->lineEditNewProfile->clear();
    //...
    int size = Config::generalConfig().beginReadArray("Osint-Profiles");
    Config::generalConfig().endArray();
    Config::generalConfig().beginWriteArray("Osint-Profiles");
    Config::generalConfig().setArrayIndex(size);
    Config::generalConfig().setValue("value", profileName);
    Config::generalConfig().endArray();
    ///
    /// saving to profiles...
    ///
    QSettings settings(m_currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.beginGroup(profileName);

/***************************************************************
                           API
****************************************************************/

    if(ui->moduleAnubis->isChecked()){
        settings.setValue("anubis", TRUE);
    }else{
        settings.setValue("anubis", FALSE);
    }
    if(ui->moduleBgpview->isChecked()){
        settings.setValue("bgpview", TRUE);
    }else{
        settings.setValue("bgpview", FALSE);
    }
    if(ui->moduleBinaryEdge->isChecked()){
        settings.setValue("binaryedge", TRUE);
    }else{
        settings.setValue("binaryedge", FALSE);
    }
    if(ui->moduleHackerTargetFree->isChecked()){
        settings.setValue("hackertargetfree", TRUE);
    }else{
        settings.setValue("hackertargetfree", FALSE);
    }
    if(ui->moduleCircl->isChecked()){
        settings.setValue("circl", TRUE);
    }else{
        settings.setValue("circl", FALSE);
    }
    if(ui->moduleHunter->isChecked()){
        settings.setValue("hunter", TRUE);
    }else{
        settings.setValue("hunter", FALSE);
    }
    if(ui->moduleMnemonicFree->isChecked()){
        settings.setValue("mnemonicfree", TRUE);
    }else{
        settings.setValue("mnemonicfree", FALSE);
    }
    if(ui->moduleMnemonicPaid->isChecked()){
        settings.setValue("mnemonicpaid", TRUE);
    }else{
        settings.setValue("mnemonicpaid", FALSE);
    }
    if(ui->moduleOmnisint->isChecked()){
        settings.setValue(OSINT_OMNISINT, TRUE);
    }else{
        settings.setValue(OSINT_OMNISINT, FALSE);
    }
    if(ui->moduleOtxFree->isChecked()){
        settings.setValue("otxfree", TRUE);
    }else{
        settings.setValue("otxfree", FALSE);
    }
    if(ui->moduleOtxPaid->isChecked()){
        settings.setValue("otxpaid", TRUE);
    }else{
        settings.setValue("otxpaid", FALSE);
    }
    if(ui->moduleProjectdiscovery->isChecked()){
        settings.setValue(OSINT_PROJECTDISCOVERY, TRUE);
    }else{
        settings.setValue(OSINT_PROJECTDISCOVERY, FALSE);
    }
    if(ui->moduleRiskIq->isChecked()){
        settings.setValue("riskiq", TRUE);
    }else{
        settings.setValue("riskiq", FALSE);
    }
    if(ui->moduleRobtexFree->isChecked()){
        settings.setValue("robtexfree", TRUE);
    }else{
        settings.setValue("robtexfree", FALSE);
    }
    if(ui->moduleRobtexPaid->isChecked()){
        settings.setValue("robtexpaid", TRUE);
    }else{
        settings.setValue("robtexpaid", FALSE);
    }
    if(ui->moduleSecuritytrails->isChecked()){
        settings.setValue(OSINT_SECURITYTRAILS, TRUE);
    }else{
        settings.setValue(OSINT_SECURITYTRAILS, FALSE);
    }
    if(ui->moduleShodan->isChecked()){
        settings.setValue(OSINT_SHODAN, TRUE);
    }else{
        settings.setValue(OSINT_SHODAN, FALSE);
    }
    if(ui->moduleSpyse->isChecked()){
        settings.setValue(OSINT_SPYSE, TRUE);
    }else{
        settings.setValue(OSINT_SPYSE, FALSE);
    }
    if(ui->moduleSublist3r->isChecked()){
        settings.setValue("sublist3r", TRUE);
    }else{
        settings.setValue("sublist3r", FALSE);
    }
    if(ui->moduleThreatBook->isChecked()){
        settings.setValue("threatbook", TRUE);
    }else{
        settings.setValue("threatbook", FALSE);
    }
    if(ui->moduleThreatcrowd->isChecked()){
        settings.setValue(OSINT_THREATCROWD, TRUE);
    }else{
        settings.setValue(OSINT_THREATCROWD, FALSE);
    }
    if(ui->moduleThreatminer->isChecked()){
        settings.setValue(OSINT_THREATMINER, TRUE);
    }else{
        settings.setValue(OSINT_THREATMINER, FALSE);
    }
    if(ui->moduleViewDns->isChecked()){
        settings.setValue("viewdns", TRUE);
    }else{
        settings.setValue("viewdns", FALSE);
    }
    if(ui->moduleWebResolver->isChecked()){
        settings.setValue("webresolver", TRUE);
    }else{
        settings.setValue("webresolver", FALSE);
    }
    if(ui->moduleWhoisXmlApi->isChecked()){
        settings.setValue("whoisxmlapi", TRUE);
    }else{
        settings.setValue("whoisxmlapi", FALSE);
    }
    if(ui->moduleZetalytics->isChecked()){
        settings.setValue("zetalytics", TRUE);
    }else{
        settings.setValue("zetalytics", FALSE);
    }
    if(ui->moduleZoomeye->isChecked()){
        settings.setValue("zoomeye", TRUE);
    }else{
        settings.setValue("zoomeye", FALSE);
    }
    if(ui->moduleIpInfo->isChecked()){
        settings.setValue("ipinfo", TRUE);
    }else{
        settings.setValue("ipinfo", FALSE);
    }
    if(ui->moduleDnslytics->isChecked()){
        settings.setValue("dnslytics", TRUE);
    }else{
        settings.setValue("dnslytics", FALSE);
    }
    if(ui->moduleMaltiverseFree->isChecked()){
        settings.setValue("maltiversefree", TRUE);
    }else{
        settings.setValue("maltiversefree", FALSE);
    }
    if(ui->moduleN45HT->isChecked()){
        settings.setValue("n45ht", TRUE);
    }else{
        settings.setValue("n45ht", FALSE);
    }
    if(ui->moduleOnyphe->isChecked()){
        settings.setValue("onyphe", TRUE);
    }else{
        settings.setValue("onyphe", FALSE);
    }
    if(ui->moduleRipe->isChecked()){
        settings.setValue("ripe", TRUE);
    }else{
        settings.setValue("ripe", FALSE);
    }
    if(ui->moduleFullHunt->isChecked()){
        settings.setValue("fullhunt", TRUE);
    }else{
        settings.setValue("fullhunt", FALSE);
    }
    if(ui->moduleNetworksDB->isChecked()){
        settings.setValue("networksdb", TRUE);
    }else{
        settings.setValue("networksdb", FALSE);
    }
    if(ui->moduleSpyOnWeb->isChecked()){
        settings.setValue("spyonweb", TRUE);
    }else{
        settings.setValue("spyonweb", FALSE);
    }

/***************************************************************
                          ARCHIVES
****************************************************************/
    if(ui->moduleArchiveit->isChecked()){
        settings.setValue("archiveit", TRUE);
    }else{
        settings.setValue("archiveit", FALSE);
    }
    if(ui->moduleArchiveToday->isChecked()){
        settings.setValue("archivetoday", TRUE);
    }else{
        settings.setValue("archivetoday", FALSE);
    }
    if(ui->moduleArquivo->isChecked()){
        settings.setValue("arquivo", TRUE);
    }else{
        settings.setValue("arquivo", FALSE);
    }
    if(ui->moduleCommonCrawl->isChecked()){
        settings.setValue("commoncrawl", TRUE);
    }else{
        settings.setValue("commoncrawl", FALSE);
    }
    if(ui->moduleUKWebArchive->isChecked()){
        settings.setValue("ukwebarchive", TRUE);
    }else{
        settings.setValue("ukwebarchive", FALSE);
    }
    if(ui->moduleWaybackmachine->isChecked()){
        settings.setValue(OSINT_WAYBACKMACHINE, TRUE);
    }else{
        settings.setValue(OSINT_WAYBACKMACHINE, FALSE);
    }

/***************************************************************
                           CERTS
****************************************************************/

    if(ui->moduleCensys->isChecked()){
        settings.setValue(OSINT_CENSYS, TRUE);
    }else{
        settings.setValue(OSINT_CENSYS, FALSE);
    }
    if(ui->moduleGithub->isChecked()){
        settings.setValue(OSINT_GITHUB, TRUE);
    }else{
        settings.setValue(OSINT_GITHUB, FALSE);
    }
    if(ui->moduleCertspotter->isChecked()){
        settings.setValue(OSINT_CERTSPOTTER, TRUE);
    }else{
        settings.setValue(OSINT_CERTSPOTTER, FALSE);
    }
    if(ui->moduleDogpile->isChecked()){
        settings.setValue(OSINT_DOGPILE, TRUE);
    }else{
        settings.setValue(OSINT_DOGPILE, FALSE);
    }
    if(ui->moduleDuckduckgo->isChecked()){
        settings.setValue(OSINT_DUCKDUCKGO, TRUE);
    }else{
        settings.setValue(OSINT_DUCKDUCKGO, FALSE);
    }
    if(ui->moduleExalead->isChecked()){
        settings.setValue(OSINT_EXALEAD, TRUE);
    }else{
        settings.setValue(OSINT_EXALEAD, FALSE);
    }
    if(ui->moduleIntelx->isChecked()){
        settings.setValue(OSINT_INTELX, TRUE);
    }else{
        settings.setValue(OSINT_INTELX, FALSE);
    }
    if(ui->moduleNetcraft->isChecked()){
        settings.setValue(OSINT_NETCRAFT, TRUE);
    }else{
        settings.setValue(OSINT_NETCRAFT, FALSE);
    }
    if(ui->moduleSuip->isChecked()){
        settings.setValue(OSINT_SUIP, TRUE);
    }else{
        settings.setValue(OSINT_SUIP, FALSE);
    }
    if(ui->moduleTrello->isChecked()){
        settings.setValue(OSINT_TRELLO, TRUE);
    }else{
        settings.setValue(OSINT_TRELLO, FALSE);
    }
    if(ui->moduleCloudflare->isChecked()){
        settings.setValue(OSINT_CLOUDFLARE, TRUE);
    }else{
        settings.setValue(OSINT_CLOUDFLARE, FALSE);
    }
    if(ui->moduleDnsbufferoverrun->isChecked()){
        settings.setValue(OSINT_DNSBUFFEROVERRUN, TRUE);
    }else{
        settings.setValue(OSINT_DNSBUFFEROVERRUN, FALSE);
    }
    if(ui->modulePkey->isChecked()){
        settings.setValue(OSINT_PKEY, TRUE);
    }else{
        settings.setValue(OSINT_PKEY, FALSE);
    }
    if(ui->moduleAsk->isChecked()){
        settings.setValue(OSINT_ASK, TRUE);
    }else{
        settings.setValue(OSINT_ASK, FALSE);
    }
    if(ui->moduleBaidu->isChecked()){
        settings.setValue(OSINT_BAIDU, TRUE);
    }else{
        settings.setValue(OSINT_BAIDU, FALSE);
    }
    if(ui->moduleBing->isChecked()){
        settings.setValue(OSINT_BING, TRUE);
    }else{
        settings.setValue(OSINT_BING, FALSE);
    }
    if(ui->moduleCrtsh->isChecked()){
        settings.setValue(OSINT_CRTSH, TRUE);
    }else{
        settings.setValue(OSINT_CRTSH, FALSE);
    }
    if(ui->moduleDnsdumpster->isChecked()){
        settings.setValue(OSINT_DNSDUMPSTER, TRUE);
    }else{
        settings.setValue(OSINT_DNSDUMPSTER, FALSE);
    }
    if(ui->moduleGoogle->isChecked()){
        settings.setValue(OSINT_GOOGLE, TRUE);
    }else{
        settings.setValue(OSINT_GOOGLE, FALSE);
    }
    if(ui->moduleYahoo->isChecked()){
        settings.setValue(OSINT_YAHOO, TRUE);
    }else{
        settings.setValue(OSINT_YAHOO, FALSE);
    }
    if(ui->moduleRapiddns->isChecked()){
        settings.setValue(OSINT_RAPIDDNS, TRUE);
    }else{
        settings.setValue(OSINT_RAPIDDNS, FALSE);
    }
    if(ui->moduleUrlscan->isChecked()){
        settings.setValue(OSINT_URLSCAN, TRUE);
    }else{
        settings.setValue(OSINT_URLSCAN, FALSE);
    }
    if(ui->modulePentesttools->isChecked()){
        settings.setValue(OSINT_PENTESTTOOLS, TRUE);
    }else{
        settings.setValue(OSINT_PENTESTTOOLS, FALSE);
    }
    settings.endGroup();
}

void Osint::on_buttonDeleteProfile_clicked(){
    ///
    /// remove the name of the profile from main config file...
    ///
    Config::generalConfig().beginWriteArray("Osint-Profiles");
    Config::generalConfig().remove(ui->comboBoxProfiles->currentText());
    Config::generalConfig().endArray();
    ///
    /// remove the entire group(profile) from profiles...
    ///
    QSettings settings(m_currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.remove(ui->comboBoxProfiles->currentText());
    ///
    /// remove name from comboBox...
    ///
    ui->comboBoxProfiles->removeItem(ui->comboBoxProfiles->currentIndex());
}

void Osint::initProfiles(){
    int size = Config::generalConfig().beginReadArray("Osint-Profiles");
    for(int i = 0; i < size; i++){
        Config::generalConfig().setArrayIndex(i);
        ui->comboBoxProfiles->addItem(Config::generalConfig().value("value").toString());
    }
    Config::generalConfig().endArray();
}
