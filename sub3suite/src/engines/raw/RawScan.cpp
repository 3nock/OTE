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
    m_scanArgs->target = ui->lineEditTarget->text();
    m_scanArgs->outputRaw = true;
    //...
    m_currentTarget = m_scanArgs->target;

    ///
    /// ...
    ///
    if(ui->moduleCertspotter->isChecked())
    {
        m_currentModule = "CertSpotter";
        this->startScanThread(new Certspotter(m_scanArgs));
        return;
    }
    if(ui->moduleCertspotterFree->isChecked())
    {
        m_currentModule = "CertspotterFree";
        this->startScanThread(new CertspotterFree(m_scanArgs));
        return;
    }
    if(ui->moduleOtx->isChecked())
    {
        m_currentModule = "Otx";
        this->startScanThread(new OtxFree(m_scanArgs));
        return;
    }
    if(ui->moduleSublist3r->isChecked())
    {
        m_currentModule = "Sublist3r";
        this->startScanThread(new Sublist3r(m_scanArgs));
        return;
    }
    if(ui->moduleThreatMiner->isChecked())
    {
        m_currentModule = "ThreatMiner";
        this->startScanThread(new Threatminer(m_scanArgs));
        return;
    }
    if(ui->moduleThreatCrowd->isChecked())
    {
        m_currentModule = "ThreatCrowd";
        this->startScanThread(new Threatcrowd(m_scanArgs));
        return;
    }
    if(ui->moduleHackerTargetFree->isChecked())
    {
        m_currentModule = "HackerTargetFree";
        this->startScanThread(new HackerTargetFree(m_scanArgs));
        return;
    }
    if(ui->moduleDnsBufferOverRun->isChecked())
    {
        m_currentModule = "DnsBufferOverRun";
        this->startScanThread(new Dnsbufferoverun(m_scanArgs));
        return;
    }
    if(ui->moduleAnubis->isChecked())
    {
        m_currentModule = "Anubis";
        this->startScanThread(new Anubis(m_scanArgs));
        return;
    }
    if(ui->moduleProjectDiscovery->isChecked())
    {
        m_currentModule = "ProjectDiscovery";
        this->startScanThread(new Projectdiscovery(m_scanArgs));
        return;
    }
    if(ui->moduleSpyse->isChecked())
    {
        m_currentModule = "Spyse";
        this->startScanThread(new Spyse(m_scanArgs));
        return;
    }
    if(ui->moduleGoogleCert->isChecked())
    {
        m_currentModule = "GoogleCert";
        this->startScanThread(new GoogleCert(m_scanArgs));
        return;
    }
    if(ui->moduleOmnisint->isChecked())
    {
        m_currentModule = "Ominisint";
        this->startScanThread(new Omnisint(m_scanArgs));
        return;
    }
    if(ui->moduleRobtexPaid->isChecked())
    {
        m_currentModule = "RobtexPaid";
        this->startScanThread(new RobtexPaid(m_scanArgs));
        return;
    }
    if(ui->moduleVirusTotal->isChecked())
    {
        m_currentModule = "VirusTotal";
        this->startScanThread(new VirusTotal(m_scanArgs));
        return;
    }
    if(ui->moduleUrlScan->isChecked())
    {
        m_currentModule = "UrlScan";
        this->startScanThread(new Urlscan(m_scanArgs));
        return;
    }
    if(ui->moduleWaybackMachine->isChecked())
    {
        m_currentModule = "WaybackMachine";
        this->startScanThread(new Waybackmachine(m_scanArgs));
        return;
    }
    if(ui->moduleArchiveIt->isChecked())
    {
        m_currentModule = "ArchiveIt";
        this->startScanThread(new ArchiveIt(m_scanArgs));
        return;
    }
    if(ui->moduleBgpview->isChecked())
    {
        m_currentModule = "Bgpview";
        this->startScanThread(new Bgpview(m_scanArgs));
        return;
    }
    if(ui->moduleBinaryEdge->isChecked())
    {
        m_currentModule = "BinaryEdge";
        this->startScanThread(new BinaryEdge(m_scanArgs));
        return;
    }
    if(ui->moduleC99->isChecked())
    {
        m_currentModule = "C99";
        this->startScanThread(new C99(m_scanArgs));
        return;
    }
    if(ui->moduleGithub->isChecked())
    {
        m_currentModule = "Gtihub";
        this->startScanThread(new Github(m_scanArgs));
        return;
    }
    if(ui->moduleHunter->isChecked())
    {
        m_currentModule = "Hunter";
        this->startScanThread(new Hunter(m_scanArgs));
        return;
    }
    if(ui->moduleIpInfo->isChecked())
    {
        m_currentModule = "IpInfo";
        this->startScanThread(new IpInfo(m_scanArgs));
        return;
    }
    if(ui->moduleMnemonicFree->isChecked())
    {
        m_currentModule = "Mnemonic";
        this->startScanThread(new MnemonicFree(m_scanArgs));
        return;
    }
    if(ui->moduleRiskIq->isChecked())
    {
        m_currentModule = "RiskIq";
        this->startScanThread(new RiskIq(m_scanArgs));
        return;
    }
    if(ui->moduleRobtex->isChecked())
    {
        m_currentModule = "RobtexFree";
        this->startScanThread(new RobtexFree(m_scanArgs));
        return;
    }
    if(ui->moduleSecurityTrails->isChecked())
    {
        m_currentModule = "SecurityTrails";
        this->startScanThread(new SecurityTrails(m_scanArgs));
        return;
    }
    if(ui->moduleShodan->isChecked())
    {
        m_currentModule = "Shodan";
        this->startScanThread(new Shodan(m_scanArgs));
        return;
    }
    if(ui->moduleThreatBook->isChecked())
    {
        m_currentModule = "ThreatBook";
        this->startScanThread(new ThreatBook(m_scanArgs));
        return;
    }
    if(ui->moduleWhoisXmlApi->isChecked())
    {
        m_currentModule = "WhoIsXmlApi";
        this->startScanThread(new WhoisXmlApi(m_scanArgs));
        return;
    }
    if(ui->moduleZETALytics->isChecked())
    {
        m_currentModule = "ZETALytics";
        this->startScanThread(new ZETAlytics(m_scanArgs));
        return;
    }
    if(ui->moduleZoomEye->isChecked())
    {
        m_currentModule = "ZoomEye";
        this->startScanThread(new ZoomEye(m_scanArgs));
        return;
    }
    if(ui->moduleIpApi->isChecked())
    {
        m_currentModule = "IpApi";
        this->startScanThread(new IpApi(m_scanArgs));
        return;
    }
    if(ui->moduleViewDns->isChecked())
    {
        m_currentModule = "ViewDns";
        this->startScanThread(new ViewDns(m_scanArgs));
        return;
    }
    if(ui->moduleWebResolver->isChecked())
    {
        m_currentModule = "WebResolver";
        this->startScanThread(new WebResolver(m_scanArgs));
        return;
    }
    if(ui->moduleCircl->isChecked())
    {
        m_currentModule = "Circl";
        this->startScanThread(new Circl(m_scanArgs));
        return;
    }
    if(ui->moduleHackerTarget->isChecked())
    {
        m_currentModule = "HackerTarget";
        this->startScanThread(new HackerTargetPaid(m_scanArgs));
        return;
    }
    if(ui->moduleMnemonic->isChecked())
    {
        m_currentModule = "Mnemonic";
        this->startScanThread(new MnemonicPaid(m_scanArgs));
        return;
    }
    if(ui->moduleOtx->isChecked())
    {
        m_currentModule = "Otx";
        this->startScanThread(new OtxPaid(m_scanArgs));
        return;
    }
    if(ui->moduleArin->isChecked())
    {
        m_currentModule = "Arin";
        this->startScanThread(new Arin(m_scanArgs));
        return;
    }
    if(ui->moduleBuiltWith->isChecked())
    {
        m_currentModule = "BuiltWith";
        this->startScanThread(new BuiltWith(m_scanArgs));
        return;
    }
    if(ui->moduleDnslytics->isChecked())
    {
        m_currentModule = "DNSlytics";
        this->startScanThread(new Dnslytics(m_scanArgs));
        return;
    }
    if(ui->moduleDomainTools->isChecked())
    {
        m_currentModule = "DomainTools";
        this->startScanThread(new DomainTools(m_scanArgs));
        return;
    }
    if(ui->moduleMaltiverseFree->isChecked())
    {
        m_currentModule = "Maltiverse";
        this->startScanThread(new Maltiverse(m_scanArgs));
        return;
    }
    if(ui->moduleN45HT->isChecked())
    {
        m_currentModule = "N45HT";
        this->startScanThread(new N45HT(m_scanArgs));
        return;
    }
    if(ui->moduleOnyphe->isChecked())
    {
        m_currentModule = "Onyphe";
        this->startScanThread(new Onyphe(m_scanArgs));
        return;
    }
    if(ui->moduleRipe->isChecked())
    {
        m_currentModule = "Ripe";
        this->startScanThread(new Ripe(m_scanArgs));
        return;
    }
    if(ui->moduleAfrinic->isChecked())
    {
        m_currentModule = "Afrinic";
        this->startScanThread(new Afrinic(m_scanArgs));
        return;
    }
    if(ui->moduleApnic->isChecked())
    {
        m_currentModule = "Apnic";
        this->startScanThread(new Apnic(m_scanArgs));
        return;
    }
    if(ui->moduleFullHunt->isChecked())
    {
        m_currentModule = "FullHunt";
        this->startScanThread(new FullHunt(m_scanArgs));
        return;
    }
    if(ui->moduleSpamHaus->isChecked())
    {
        m_currentModule = "SpamHaus";
        this->startScanThread(new SpamHaus(m_scanArgs));
        return;
    }
    if(ui->moduleDebounce->isChecked())
    {
        m_currentModule = "Debounce";
        this->startScanThread(new Debounce(m_scanArgs));
        return;
    }
    if(ui->moduleHaveIBeenPawned->isChecked())
    {
        m_currentModule = "HaveIBeenPawned";
        this->startScanThread(new HaveIBeenPawned(m_scanArgs));
        return;
    }
    if(ui->moduleHost->isChecked())
    {
        m_currentModule = "Host";
        this->startScanThread(new Host(m_scanArgs));
        return;
    }
    if(ui->moduleJsonWhois->isChecked())
    {
        m_currentModule = "JsonWhois";
        this->startScanThread(new JsonWhois(m_scanArgs));
        return;
    }
    if(ui->moduleLeakIX->isChecked())
    {
        m_currentModule = "LeakIX";
        this->startScanThread(new LeakIX(m_scanArgs));
        return;
    }
    if(ui->moduleNetworksDB->isChecked())
    {
        m_currentModule = "NetworksDB";
        this->startScanThread(new NetworksDB(m_scanArgs));
        return;
    }
    if(ui->moduleNeutrinoApi->isChecked())
    {
        m_currentModule = "NeutrinoApi";
        this->startScanThread(new NeutrinoApi(m_scanArgs));
        return;
    }
    if(ui->moduleSeon->isChecked())
    {
        m_currentModule = "Seon";
        this->startScanThread(new Seon(m_scanArgs));
        return;
    }
    if(ui->moduleSpyOnWeb->isChecked())
    {
        m_currentModule = "SpyOnWeb";
        this->startScanThread(new SpyOnWeb(m_scanArgs));
        return;
    }
    if(ui->moduleWhatcms->isChecked())
    {
        m_currentModule = "WhatCMS";
        this->startScanThread(new Whatcms(m_scanArgs));
        return;
    }
    if(ui->moduleWhoxy->isChecked())
    {
        m_currentModule = "Whoxy";
        this->startScanThread(new Whoxy(m_scanArgs));
        return;
    }
    if(ui->moduleArquivo->isChecked())
    {
        m_currentModule = "Arquivo";
        this->startScanThread(new Arquivo(m_scanArgs));
        return;
    }
    if(ui->moduleUKWebArchive->isChecked())
    {
        m_currentModule = "UKWebArchive";
        this->startScanThread(new UKWebArchive(m_scanArgs));
        return;
    }
    if(ui->moduleCensys->isChecked())
    {
        m_currentModule = "Censys";
        this->startScanThread(new Censys(m_scanArgs));
        return;
    }
    if(ui->moduleEmailCrawlr->isChecked())
    {
        m_currentModule = "EmailCrawlr";
        this->startScanThread(new EmailCrawlr(m_scanArgs));
        return;
    }
    if(ui->moduleEmailFormat->isChecked())
    {
        m_currentModule = "EmailFormat";
        this->startScanThread(new EmailFormat(m_scanArgs));
        return;
    }
    if(ui->moduleEmailRep->isChecked())
    {
        m_currentModule = "EmailRep";
        this->startScanThread(new EmailRep(m_scanArgs));
        return;
    }
    if(ui->moduleSnov->isChecked())
    {
        m_currentModule = "Snov";
        this->startScanThread(new Snov(m_scanArgs));
        return;
    }
    if(ui->moduleTruMail->isChecked())
    {
        m_currentModule = "TruMail";
        this->startScanThread(new TruMail(m_scanArgs));
        return;
    }
    if(ui->moduleIpData->isChecked())
    {
        m_currentModule = "IpData";
        this->startScanThread(new IpData(m_scanArgs));
        return;
    }
    if(ui->moduleIpGeoLocation->isChecked())
    {
        m_currentModule = "IpGeoLocation";
        this->startScanThread(new IpGeoLocation(m_scanArgs));
        return;
    }
    if(ui->moduleIpRegistry->isChecked())
    {
        m_currentModule = "IpRegistry";
        this->startScanThread(new IpRegistry(m_scanArgs));
        return;
    }
    if(ui->moduleIpStack->isChecked())
    {
        m_currentModule = "IpStack";
        this->startScanThread(new IpStack(m_scanArgs));
        return;
    }
    if(ui->moduleAbuseIPDB->isChecked())
    {
        m_currentModule = "AbuseIPDB";
        this->startScanThread(new AbuseIPDB(m_scanArgs));
        return;
    }
    if(ui->moduleBotScout->isChecked())
    {
        m_currentModule = "BotScout";
        this->startScanThread(new BotScout(m_scanArgs));
        return;
    }
    if(ui->moduleFraudGuard->isChecked())
    {
        m_currentModule = "FraudGuard";
        this->startScanThread(new FraudGuard(m_scanArgs));
        return;
    }
    if(ui->moduleHybridAnalysis->isChecked())
    {
        m_currentModule = "HybridAnalysis";
        this->startScanThread(new HybridAnalysis(m_scanArgs));
        return;
    }
    if(ui->moduleIpQualityScore->isChecked())
    {
        m_currentModule = "IpQualityScore";
        this->startScanThread(new IpQualityScore(m_scanArgs));
        return;
    }
    if(ui->moduleLeakLookup->isChecked())
    {
        m_currentModule = "Leak-Lookup";
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
