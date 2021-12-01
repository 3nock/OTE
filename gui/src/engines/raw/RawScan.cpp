#include "Raw.h"
#include "ui_Raw.h"
#include <QMessageBox>


/**************************************************************************
*
*
*
*
***************************************************************************/


void Raw::startScan(){
    m_scanArgs->rawOption = ui->comboBoxOptions->currentIndex();
    m_scanArgs->option = ui->comboBoxOptions->currentText();
    m_scanArgs->target = ui->lineEditTarget->text();
    m_scanArgs->raw = true;

    if(ui->moduleCertspotter->isChecked())
    {
        m_scanArgs->module = "CertSpotter";
        this->startScanThread(new Certspotter(m_scanArgs));
        return;
    }
    if(ui->moduleCertspotterFree->isChecked())
    {
        m_scanArgs->module = "CertspotterFree";
        this->startScanThread(new CertspotterFree(m_scanArgs));
        return;
    }
    if(ui->moduleOtx->isChecked())
    {
        m_scanArgs->module = "Otx";
        this->startScanThread(new OtxFree(m_scanArgs));
        return;
    }
    if(ui->moduleSublist3r->isChecked())
    {
        m_scanArgs->module = "Sublist3r";
        this->startScanThread(new Sublist3r(m_scanArgs));
        return;
    }
    if(ui->moduleThreatMiner->isChecked())
    {
        m_scanArgs->module = "ThreatMiner";
        this->startScanThread(new Threatminer(m_scanArgs));
        return;
    }
    if(ui->moduleThreatCrowd->isChecked())
    {
        m_scanArgs->module = "ThreatCrowd";
        this->startScanThread(new Threatcrowd(m_scanArgs));
        return;
    }
    if(ui->moduleHackerTargetFree->isChecked())
    {
        m_scanArgs->module = "HackerTargetFree";
        this->startScanThread(new HackerTargetFree(m_scanArgs));
        return;
    }
    if(ui->moduleDnsBufferOverRun->isChecked())
    {
        m_scanArgs->module = "DnsBufferOverRun";
        this->startScanThread(new Dnsbufferoverun(m_scanArgs));
        return;
    }
    if(ui->moduleAnubis->isChecked())
    {
        m_scanArgs->module = "Anubis";
        this->startScanThread(new Anubis(m_scanArgs));
        return;
    }
    if(ui->moduleProjectDiscovery->isChecked())
    {
        m_scanArgs->module = "ProjectDiscovery";
        this->startScanThread(new Projectdiscovery(m_scanArgs));
        return;
    }
    if(ui->moduleSpyse->isChecked())
    {
        m_scanArgs->module = "Spyse";
        this->startScanThread(new Spyse(m_scanArgs));
        return;
    }
    if(ui->moduleGoogleCert->isChecked())
    {
        m_scanArgs->module = "GoogleCert";
        this->startScanThread(new GoogleCert(m_scanArgs));
        return;
    }
    if(ui->moduleOmnisint->isChecked())
    {
        m_scanArgs->module = "Ominisint";
        this->startScanThread(new Omnisint(m_scanArgs));
        return;
    }
    if(ui->moduleRobtexPaid->isChecked())
    {
        m_scanArgs->module = "RobtexPaid";
        this->startScanThread(new RobtexPaid(m_scanArgs));
        return;
    }
    if(ui->moduleVirusTotal->isChecked())
    {
        m_scanArgs->module = "VirusTotal";
        this->startScanThread(new VirusTotal(m_scanArgs));
        return;
    }
    if(ui->moduleUrlScan->isChecked())
    {
        m_scanArgs->module = "UrlScan";
        this->startScanThread(new Urlscan(m_scanArgs));
        return;
    }
    if(ui->moduleWaybackMachine->isChecked())
    {
        m_scanArgs->module = "WaybackMachine";
        this->startScanThread(new Waybackmachine(m_scanArgs));
        return;
    }
    if(ui->moduleArchiveIt->isChecked())
    {
        m_scanArgs->module = "ArchiveIt";
        this->startScanThread(new ArchiveIt(m_scanArgs));
        return;
    }
    if(ui->moduleBgpview->isChecked())
    {
        m_scanArgs->module = "Bgpview";
        this->startScanThread(new Bgpview(m_scanArgs));
        return;
    }
    if(ui->moduleBinaryEdge->isChecked())
    {
        m_scanArgs->module = "BinaryEdge";
        this->startScanThread(new BinaryEdge(m_scanArgs));
        return;
    }
    if(ui->moduleC99->isChecked())
    {
        m_scanArgs->module = "C99";
        this->startScanThread(new C99(m_scanArgs));
        return;
    }
    if(ui->moduleGithub->isChecked())
    {
        m_scanArgs->module = "Gtihub";
        this->startScanThread(new Github(m_scanArgs));
        return;
    }
    if(ui->moduleHunter->isChecked())
    {
        m_scanArgs->module = "Hunter";
        this->startScanThread(new Hunter(m_scanArgs));
        return;
    }
    if(ui->moduleIpInfo->isChecked())
    {
        m_scanArgs->module = "IpInfo";
        this->startScanThread(new IpInfo(m_scanArgs));
        return;
    }
    if(ui->moduleMnemonicFree->isChecked())
    {
        m_scanArgs->module = "Mnemonic";
        this->startScanThread(new MnemonicFree(m_scanArgs));
        return;
    }
    if(ui->moduleRiskIq->isChecked())
    {
        m_scanArgs->module = "RiskIq";
        this->startScanThread(new RiskIq(m_scanArgs));
        return;
    }
    if(ui->moduleRobtex->isChecked())
    {
        m_scanArgs->module = "RobtexFree";
        this->startScanThread(new RobtexFree(m_scanArgs));
        return;
    }
    if(ui->moduleSecurityTrails->isChecked())
    {
        m_scanArgs->module = "SecurityTrails";
        this->startScanThread(new SecurityTrails(m_scanArgs));
        return;
    }
    if(ui->moduleShodan->isChecked())
    {
        m_scanArgs->module = "Shodan";
        this->startScanThread(new Shodan(m_scanArgs));
        return;
    }
    if(ui->moduleThreatBook->isChecked())
    {
        m_scanArgs->module = "ThreatBook";
        this->startScanThread(new ThreatBook(m_scanArgs));
        return;
    }
    if(ui->moduleWhoisXmlApi->isChecked())
    {
        m_scanArgs->module = "WhoIsXmlApi";
        this->startScanThread(new WhoisXmlApi(m_scanArgs));
        return;
    }
    if(ui->moduleZETALytics->isChecked())
    {
        m_scanArgs->module = "ZETALytics";
        this->startScanThread(new ZETAlytics(m_scanArgs));
        return;
    }
    if(ui->moduleZoomEye->isChecked())
    {
        m_scanArgs->module = "ZoomEye";
        this->startScanThread(new ZoomEye(m_scanArgs));
        return;
    }
    if(ui->moduleIpApi->isChecked())
    {
        m_scanArgs->module = "IpApi";
        this->startScanThread(new IpApi(m_scanArgs));
        return;
    }
    if(ui->moduleViewDns->isChecked())
    {
        m_scanArgs->module = "ViewDns";
        this->startScanThread(new ViewDns(m_scanArgs));
        return;
    }
    if(ui->moduleWebResolver->isChecked())
    {
        m_scanArgs->module = "WebResolver";
        this->startScanThread(new WebResolver(m_scanArgs));
        return;
    }
    if(ui->moduleCircl->isChecked())
    {
        m_scanArgs->module = "Circl";
        this->startScanThread(new Circl(m_scanArgs));
        return;
    }
    if(ui->moduleHackerTarget->isChecked())
    {
        m_scanArgs->module = "HackerTarget";
        this->startScanThread(new HackerTargetPaid(m_scanArgs));
        return;
    }
    if(ui->moduleMnemonic->isChecked())
    {
        m_scanArgs->module = "Mnemonic";
        this->startScanThread(new MnemonicPaid(m_scanArgs));
        return;
    }
    if(ui->moduleOtx->isChecked())
    {
        m_scanArgs->module = "Otx";
        this->startScanThread(new OtxPaid(m_scanArgs));
        return;
    }
    if(ui->moduleArin->isChecked())
    {
        m_scanArgs->module = "Arin";
        this->startScanThread(new Arin(m_scanArgs));
        return;
    }
    if(ui->moduleBuiltWith->isChecked())
    {
        m_scanArgs->module = "BuiltWith";
        this->startScanThread(new BuiltWith(m_scanArgs));
        return;
    }
    if(ui->moduleDnslytics->isChecked())
    {
        m_scanArgs->module = "DNSlytics";
        this->startScanThread(new Dnslytics(m_scanArgs));
        return;
    }
    if(ui->moduleDomainTools->isChecked())
    {
        m_scanArgs->module = "DomainTools";
        this->startScanThread(new DomainTools(m_scanArgs));
        return;
    }
    if(ui->moduleMaltiverseFree->isChecked())
    {
        m_scanArgs->module = "Maltiverse";
        this->startScanThread(new Maltiverse(m_scanArgs));
        return;
    }
    if(ui->moduleN45HT->isChecked())
    {
        m_scanArgs->module = "N45HT";
        this->startScanThread(new N45HT(m_scanArgs));
        return;
    }
    if(ui->moduleOnyphe->isChecked())
    {
        m_scanArgs->module = "Onyphe";
        this->startScanThread(new Onyphe(m_scanArgs));
        return;
    }
    if(ui->moduleRipe->isChecked())
    {
        m_scanArgs->module = "Ripe";
        this->startScanThread(new Ripe(m_scanArgs));
        return;
    }
    if(ui->moduleAfrinic->isChecked())
    {
        m_scanArgs->module = "Afrinic";
        this->startScanThread(new Afrinic(m_scanArgs));
        return;
    }
    if(ui->moduleApnic->isChecked())
    {
        m_scanArgs->module = "Apnic";
        this->startScanThread(new Apnic(m_scanArgs));
        return;
    }
    if(ui->moduleFullHunt->isChecked())
    {
        m_scanArgs->module = "FullHunt";
        this->startScanThread(new FullHunt(m_scanArgs));
        return;
    }
    if(ui->moduleSpamHaus->isChecked())
    {
        m_scanArgs->module = "SpamHaus";
        this->startScanThread(new SpamHaus(m_scanArgs));
        return;
    }
    if(ui->moduleDebounce->isChecked())
    {
        m_scanArgs->module = "Debounce";
        this->startScanThread(new Debounce(m_scanArgs));
        return;
    }
    if(ui->moduleHaveIBeenPawned->isChecked())
    {
        m_scanArgs->module = "HaveIBeenPawned";
        this->startScanThread(new HaveIBeenPawned(m_scanArgs));
        return;
    }
    if(ui->moduleHost->isChecked())
    {
        m_scanArgs->module = "Host";
        this->startScanThread(new Host(m_scanArgs));
        return;
    }
    if(ui->moduleJsonWhois->isChecked())
    {
        m_scanArgs->module = "JsonWhois";
        this->startScanThread(new JsonWhois(m_scanArgs));
        return;
    }
    if(ui->moduleLeakIX->isChecked())
    {
        m_scanArgs->module = "LeakIX";
        this->startScanThread(new LeakIX(m_scanArgs));
        return;
    }
    if(ui->moduleNetworksDB->isChecked())
    {
        m_scanArgs->module = "NetworksDB";
        this->startScanThread(new NetworksDB(m_scanArgs));
        return;
    }
    if(ui->moduleNeutrinoApi->isChecked())
    {
        m_scanArgs->module = "NeutrinoApi";
        this->startScanThread(new NeutrinoApi(m_scanArgs));
        return;
    }
    if(ui->moduleSeon->isChecked())
    {
        m_scanArgs->module = "Seon";
        this->startScanThread(new Seon(m_scanArgs));
        return;
    }
    if(ui->moduleSpyOnWeb->isChecked())
    {
        m_scanArgs->module = "SpyOnWeb";
        this->startScanThread(new SpyOnWeb(m_scanArgs));
        return;
    }
    if(ui->moduleWhatcms->isChecked())
    {
        m_scanArgs->module = "WhatCMS";
        this->startScanThread(new Whatcms(m_scanArgs));
        return;
    }
    if(ui->moduleWhoxy->isChecked())
    {
        m_scanArgs->module = "Whoxy";
        this->startScanThread(new Whoxy(m_scanArgs));
        return;
    }
    if(ui->moduleArquivo->isChecked())
    {
        m_scanArgs->module = "Arquivo";
        this->startScanThread(new Arquivo(m_scanArgs));
        return;
    }
    if(ui->moduleUKWebArchive->isChecked())
    {
        m_scanArgs->module = "UKWebArchive";
        this->startScanThread(new UKWebArchive(m_scanArgs));
        return;
    }
    if(ui->moduleCensys->isChecked())
    {
        m_scanArgs->module = "Censys";
        this->startScanThread(new Censys(m_scanArgs));
        return;
    }
    if(ui->moduleEmailCrawlr->isChecked())
    {
        m_scanArgs->module = "EmailCrawlr";
        this->startScanThread(new EmailCrawlr(m_scanArgs));
        return;
    }
    if(ui->moduleEmailFormat->isChecked())
    {
        m_scanArgs->module = "EmailFormat";
        this->startScanThread(new EmailFormat(m_scanArgs));
        return;
    }
    if(ui->moduleEmailRep->isChecked())
    {
        m_scanArgs->module = "EmailRep";
        this->startScanThread(new EmailRep(m_scanArgs));
        return;
    }
    if(ui->moduleSnov->isChecked())
    {
        m_scanArgs->module = "Snov";
        this->startScanThread(new Snov(m_scanArgs));
        return;
    }
    if(ui->moduleTruMail->isChecked())
    {
        m_scanArgs->module = "TruMail";
        this->startScanThread(new TruMail(m_scanArgs));
        return;
    }
    if(ui->moduleIpData->isChecked())
    {
        m_scanArgs->module = "IpData";
        this->startScanThread(new IpData(m_scanArgs));
        return;
    }
    if(ui->moduleIpGeoLocation->isChecked())
    {
        m_scanArgs->module = "IpGeoLocation";
        this->startScanThread(new IpGeoLocation(m_scanArgs));
        return;
    }
    if(ui->moduleIpRegistry->isChecked())
    {
        m_scanArgs->module = "IpRegistry";
        this->startScanThread(new IpRegistry(m_scanArgs));
        return;
    }
    if(ui->moduleIpStack->isChecked())
    {
        m_scanArgs->module = "IpStack";
        this->startScanThread(new IpStack(m_scanArgs));
        return;
    }
    if(ui->moduleAbuseIPDB->isChecked())
    {
        m_scanArgs->module = "AbuseIPDB";
        this->startScanThread(new AbuseIPDB(m_scanArgs));
        return;
    }
    if(ui->moduleBotScout->isChecked())
    {
        m_scanArgs->module = "BotScout";
        this->startScanThread(new BotScout(m_scanArgs));
        return;
    }
    if(ui->moduleFraudGuard->isChecked())
    {
        m_scanArgs->module = "FraudGuard";
        this->startScanThread(new FraudGuard(m_scanArgs));
        return;
    }
    if(ui->moduleHybridAnalysis->isChecked())
    {
        m_scanArgs->module = "HybridAnalysis";
        this->startScanThread(new HybridAnalysis(m_scanArgs));
        return;
    }
    if(ui->moduleIpQualityScore->isChecked())
    {
        m_scanArgs->module = "IpQualityScore";
        this->startScanThread(new IpQualityScore(m_scanArgs));
        return;
    }
    if(ui->moduleLeakLookup->isChecked())
    {
        m_scanArgs->module = "Leak-Lookup";
        this->startScanThread(new LeakLookup(m_scanArgs));
        return;
    }

    /* if control reaches here means no module was selected... */
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
    QMessageBox::warning(this, "Error", "Please Chooose Osint Engine To Query...");
}

void Raw::startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread(this);
    module->Enumerator(cThread);
    module->moveToThread(cThread);
    //...
    connect(module, &AbstractOsintModule::rawResults, this, &Raw::onResults);
    connect(module, &AbstractOsintModule::rawResultsTxt, this, &Raw::onResultsTxt);
    connect(module, &AbstractOsintModule::errorLog, this, &Raw::onErrorLog);
    connect(module, &AbstractOsintModule::infoLog, this, &Raw::onInfoLog);
    connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
    cThread->start();
}
