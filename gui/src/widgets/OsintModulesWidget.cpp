#include "OsintModulesWidget.h"
#include "ui_OsintModulesWidget.h"
//...
#include "src/utils/utils.h"
#include "src/utils/Config.h"


OsintModulesWidget::OsintModulesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OsintModulesWidget)
{
    ui->setupUi(this);
    ///
    ///...
    ///
    ui->lineEditNewProfile->setPlaceholderText("Enter New Profile's Name...");
    ui->frameProfiles->hide();
    this->initProfiles();
}
OsintModulesWidget::~OsintModulesWidget(){
    delete ui;
}

void OsintModulesWidget::getChoosenModules(OsintModules &modules){
    /*
     * ...
     */
    if(ui->moduleAnubis->isChecked()){
        modules.anubis = true;
    }else{
        modules.anubis = false;
    }
    if(ui->moduleBgpview->isChecked()){
        modules.bgpview = true;
    }else{
        modules.bgpview = false;
    }
    if(ui->moduleBinaryEdge->isChecked()){
        modules.binaryEdge = true;
    }else{
        modules.binaryEdge = false;
    }
    if(ui->moduleHackerTargetFree->isChecked()){
        modules.hackertargetfree = true;
    }else{
        modules.hackertargetfree = false;
    }
    if(ui->moduleHackerTargetPaid->isChecked()){
        modules.hackertargetpaid = true;
    }else{
        modules.hackertargetpaid = false;
    }
    /*
     * ...
     */
    if(ui->moduleCensys->isChecked()){
        modules.censysFree = true;
    }else{
        modules.censysFree = false;
    }
    if(ui->moduleThreatminer->isChecked()){
        modules.threatminer = true;
    }else{
        modules.threatminer = false;
    }
    if(ui->moduleShodan->isChecked()){
        modules.shodan = true;
    }else{
        modules.shodan = false;
    }
    if(ui->moduleGithub->isChecked()){
        modules.github = true;
    }else{
        modules.github = false;
    }
    if(ui->moduleCertspotter->isChecked()){
        modules.certspotter = true;
    }else{
        modules.certspotter = false;
    }
    if(ui->moduleDogpile->isChecked()){
        modules.dogpile = true;
    }else{
        modules.dogpile = false;
    }
    if(ui->moduleDuckduckgo->isChecked()){
        modules.duckduckgo = true;
    }else{
        modules.duckduckgo = false;
    }
    if(ui->moduleExalead->isChecked()){
        modules.exalead = true;
    }else{
        modules.exalead = false;
    }
    if(ui->moduleHuntersearch->isChecked()){
        modules.huntersearch = true;
    }else{
        modules.huntersearch = false;
    }
    if(ui->moduleIntelx->isChecked()){
        modules.intelx = true;
    }else{
        modules.intelx = false;
    }
    if(ui->moduleNetcraft->isChecked()){
        modules.netcraft = true;
    }else{
        modules.netcraft = false;
    }
    if(ui->moduleOtx->isChecked()){
        modules.otx = true;
    }else{
        modules.otx = false;
    }
    if(ui->moduleSecuritytrails->isChecked()){
        modules.securitytrails = true;
    }else{
        modules.securitytrails = false;
    }
    if(ui->moduleSuip->isChecked()){
        modules.suip = true;
    }else{
        modules.suip = false;
    }
    if(ui->moduleTrello->isChecked()){
        modules.trello = true;
    }else{
        modules.trello = false;
    }
    if(ui->moduleCloudflare->isChecked()){
        modules.cloudflare = true;
    }else{
        modules.cloudflare = false;
    }
    if(ui->moduleThreatcrowd->isChecked()){
        modules.threatcrowd = true;
    }else{
        modules.threatcrowd = false;
    }
    if(ui->moduleDnsbufferoverrun->isChecked()){
        modules.dnsbufferoverrun = true;
    }else{
        modules.dnsbufferoverrun = false;
    }
    if(ui->modulePkey->isChecked()){
        modules.pkey = true;
    }else{
        modules.pkey = false;
    }
    if(ui->moduleWaybackmachine->isChecked()){
        modules.waybackmachine = true;
    }else{
        modules.waybackmachine = false;
    }
    if(ui->moduleAsk->isChecked()){
        modules.ask = true;
    }else{
        modules.ask = false;
    }
    if(ui->moduleBaidu->isChecked()){
        modules.baidu = true;
    }else{
        modules.baidu = false;
    }
    if(ui->moduleBing->isChecked()){
        modules.bing = true;
    }else{
        modules.bing = false;
    }
    if(ui->moduleCrtsh->isChecked()){
        modules.crtsh = true;
    }else{
        modules.crtsh = false;
    }
    if(ui->moduleDnsdumpster->isChecked()){
        modules.dnsdumpster = true;
    }else{
        modules.dnsdumpster = false;
    }
    if(ui->moduleGoogle->isChecked()){
        modules.google = true;
    }else{
        modules.google = false;
    }
    if(ui->moduleYahoo->isChecked()){
        modules.yahoo = true;
    }else{
        modules.yahoo = false;
    }
    if(ui->moduleVirustotalapi->isChecked()){
        modules.virustotalapi = true;
    }else{
        modules.virustotalapi = false;
    }
    if(ui->moduleOmnisint->isChecked()){
        modules.omnisint = true;
    }else{
        modules.omnisint = false;
    }
    if(ui->moduleQwant->isChecked()){
        modules.qwant = true;
    }else{
        modules.qwant = false;
    }
    if(ui->moduleRapiddns->isChecked()){
        modules.rapiddns = true;
    }else{
        modules.rapiddns = false;
    }
    if(ui->moduleUrlscan->isChecked()){
        modules.urlscan = true;
    }else{
        modules.urlscan = false;
    }
    if(ui->modulePentesttools->isChecked()){
        modules.pentesttools = true;
    }else{
        modules.pentesttools = false;
    }
    if(ui->moduleProjectdiscovery->isChecked()){
        modules.projectdiscovery = true;
    }else{
        modules.projectdiscovery = false;
    }
    if(ui->moduleSpyse->isChecked()){
        modules.spyse = true;
    }else{
        modules.spyse = false;
    }
}

void OsintModulesWidget::on_useProfiles_clicked(bool checked){
    if(checked){
        ui->frameProfiles->show();
    }else{
        ui->frameProfiles->hide();
    }
}

void OsintModulesWidget::on_buttonLoadProfile_clicked(){
    ///
    /// opening the file containing the profiles...
    ///
    QSettings settings(currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.beginGroup(ui->comboBoxProfiles->currentText());
    ///
    /// reading from file...
    ///
    /*
     * ...
     */
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
    /*
     * ...
     */
    if(settings.value(OSINT_THREATMINER).toString() == TRUE){
        ui->moduleThreatminer->setChecked(true);
    }else{
        ui->moduleThreatminer->setChecked(false);
    }
    if(settings.value(OSINT_SHODAN).toString() == TRUE){
        ui->moduleShodan->setChecked(true);
    }else{
        ui->moduleShodan->setChecked(false);
    }
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
    if(settings.value(OSINT_SECURITYTRAILS).toString() == TRUE){
        ui->moduleSecuritytrails->setChecked(true);
    }else{
        ui->moduleSecuritytrails->setChecked(false);
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
    if(settings.value(OSINT_HUNTERSEARCH).toString() == TRUE){
        ui->moduleHuntersearch->setChecked(true);
    }else{
        ui->moduleHuntersearch->setChecked(false);
    }
    if(settings.value(OSINT_NETCRAFT).toString() == TRUE){
        ui->moduleNetcraft->setChecked(true);
    }else{
        ui->moduleNetcraft->setChecked(false);
    }
    if(settings.value(OSINT_OTX).toString() == TRUE){
        ui->moduleOtx->setChecked(true);
    }else{
        ui->moduleOtx->setChecked(false);
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
    if(settings.value(OSINT_THREATCROWD).toString() == TRUE){
        ui->moduleThreatcrowd->setChecked(true);
    }else{
        ui->moduleThreatcrowd->setChecked(false);
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
    if(settings.value(OSINT_WAYBACKMACHINE).toString() == TRUE){
        ui->moduleWaybackmachine->setChecked(true);
    }else{
        ui->moduleWaybackmachine->setChecked(false);
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
    if(settings.value(OSINT_VIRUSTOTALAPI).toString() == TRUE){
        ui->moduleVirustotalapi->setChecked(true);
    }else{
        ui->moduleVirustotalapi->setChecked(false);
    }
    if(settings.value(OSINT_OMNISINT).toString() == TRUE){
        ui->moduleOmnisint->setChecked(true);
    }else{
        ui->moduleOmnisint->setChecked(false);
    }
    if(settings.value(OSINT_QWANT).toString() == TRUE){
        ui->moduleQwant->setChecked(true);
    }
    else{
        ui->moduleQwant->setChecked(false);
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
    if(settings.value(OSINT_PROJECTDISCOVERY).toString() == TRUE){
        ui->moduleProjectdiscovery->setChecked(true);
    }
    else{
        ui->moduleProjectdiscovery->setChecked(false);
    }
    if(settings.value(OSINT_PENTESTTOOLS).toString() == TRUE){
        ui->modulePentesttools->setChecked(true);
    }else{
        ui->modulePentesttools->setChecked(false);
    }
    if(settings.value(OSINT_SPYSE).toString() == TRUE){
        ui->moduleSpyse->setChecked(true);
    }else{
        ui->moduleSpyse->setChecked(false);
    }
    settings.endGroup();
}

void OsintModulesWidget::on_buttonCreateProfile_clicked(){
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
    QSettings settings(currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.beginGroup(profileName);
    /*
     * ...
     */
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
    /*
     * ...
     */
    if(ui->moduleCensys->isChecked()){
        settings.setValue(OSINT_CENSYS, TRUE);
    }else{
        settings.setValue(OSINT_CENSYS, FALSE);
    }
    if(ui->moduleThreatminer->isChecked()){
        settings.setValue(OSINT_THREATMINER, TRUE);
    }else{
        settings.setValue(OSINT_THREATMINER, FALSE);
    }
    if(ui->moduleShodan->isChecked()){
        settings.setValue(OSINT_SHODAN, TRUE);
    }else{
        settings.setValue(OSINT_SHODAN, FALSE);
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
    if(ui->moduleHuntersearch->isChecked()){
        settings.setValue(OSINT_HUNTERSEARCH, TRUE);
    }else{
        settings.setValue(OSINT_HUNTERSEARCH, FALSE);
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
    if(ui->moduleOtx->isChecked()){
        settings.setValue(OSINT_OTX, TRUE);
    }else{
        settings.setValue(OSINT_OTX, FALSE);
    }
    if(ui->moduleSecuritytrails->isChecked()){
        settings.setValue(OSINT_SECURITYTRAILS, TRUE);
    }else{
        settings.setValue(OSINT_SECURITYTRAILS, FALSE);
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
    if(ui->moduleThreatcrowd->isChecked()){
        settings.setValue(OSINT_THREATCROWD, TRUE);
    }else{
        settings.setValue(OSINT_THREATCROWD, FALSE);
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
    if(ui->moduleWaybackmachine->isChecked()){
        settings.setValue(OSINT_WAYBACKMACHINE, TRUE);
    }else{
        settings.setValue(OSINT_WAYBACKMACHINE, FALSE);
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
    if(ui->moduleVirustotalapi->isChecked()){
        settings.setValue(OSINT_VIRUSTOTALAPI, TRUE);
    }else{
        settings.setValue(OSINT_VIRUSTOTALAPI, FALSE);
    }
    if(ui->moduleOmnisint->isChecked()){
        settings.setValue(OSINT_OMNISINT, TRUE);
    }else{
        settings.setValue(OSINT_OMNISINT, FALSE);
    }
    if(ui->moduleQwant->isChecked()){
        settings.setValue(OSINT_QWANT, TRUE);
    }else{
        settings.setValue(OSINT_QWANT, FALSE);
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
    if(ui->moduleProjectdiscovery->isChecked()){
        settings.setValue(OSINT_PROJECTDISCOVERY, TRUE);
    }else{
        settings.setValue(OSINT_PROJECTDISCOVERY, FALSE);
    }
    if(ui->moduleSpyse->isChecked()){
        settings.setValue(OSINT_SPYSE, TRUE);
    }else{
        settings.setValue(OSINT_SPYSE, FALSE);
    }
    settings.endGroup();
}

void OsintModulesWidget::on_buttonDeleteProfile_clicked(){
    ///
    /// remove the name of the profile from main config file...
    ///
    Config::generalConfig().beginWriteArray("Osint-Profiles");
    Config::generalConfig().remove(ui->comboBoxProfiles->currentText());
    Config::generalConfig().endArray();
    ///
    /// remove the entire group(profile) from profiles...
    ///
    QSettings settings(currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.remove(ui->comboBoxProfiles->currentText());
    ///
    /// remove name from comboBox...
    ///
    ui->comboBoxProfiles->removeItem(ui->comboBoxProfiles->currentIndex());
}

void OsintModulesWidget::initProfiles(){
    int size = Config::generalConfig().beginReadArray("Osint-Profiles");
    for(int i = 0; i < size; i++){
        Config::generalConfig().setArrayIndex(i);
        ui->comboBoxProfiles->addItem(Config::generalConfig().value("value").toString());
    }
    Config::generalConfig().endArray();
}
