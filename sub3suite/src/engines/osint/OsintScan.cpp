#include "Osint.h"
#include "ui_Osint.h"
/* ... */
#include "src/modules/osint/OsintModulesHeaders.h"


void Osint::startScan(){
    ScanArgs scanArgs;

    ///
    /// get the targets...
    ///
    if(ui->checkBoxMultipleTargets->isChecked()){
        // get all the targets...
    }
    else{
        scanArgs.targets.push(ui->lineEditTarget->text());
    }

    ///
    /// getting input type as specified by user...
    ///
    switch(ui->comboBoxInput->currentIndex()){
    case INPUT_DOMAIN:
        scanArgs.inputDomain = true;
        break;
    case INPUT_IP:
        scanArgs.inputIp = true;
        break;
    case INPUT_EMAIL:
        scanArgs.inputEmail = true;
        break;
    case INPUT_URL:
        scanArgs.inputUrl = true;
        break;
    case INPUT_ASN:
        scanArgs.inputAsn = true;
        break;
    case INPUT_SSLCERT:
        scanArgs.inputSSLCert = true;
        break;
    case INPUT_CIDR:
        scanArgs.inputCidr = true;
        break;
    }

    ///
    /// getting output type as specified by user...
    ///
    switch(ui->comboBoxOutput->currentIndex()){
    case OUTPUT_SUBDOMAINIP:
        scanArgs.outputSubdomainIp = true;
        break;
    case OUTPUT_SUBDOMAIN:
        scanArgs.outputSubdomain = true;
        break;
    case OUTPUT_IP:
        scanArgs.outputIp = true;
        break;
    case OUTPUT_EMAIL:
        scanArgs.outputEmail = true;
        break;
    case OUTPUT_URL:
        scanArgs.outputUrl = true;
        break;
    case OUTPUT_ASN:
        scanArgs.outputAsn = true;
        break;
    case OUTPUT_SSLCERT:
        scanArgs.outputSSLCert = true;
        break;
    case OUTPUT_CIDR:
        scanArgs.outputCidr = true;
        break;
    }

    /****************************************************************************
                                    APIs
    *****************************************************************************/

    if(ui->moduleAnubis->isChecked())
        this->startScanThread(new Anubis(scanArgs));

    if(ui->moduleBgpview->isChecked())
        this->startScanThread(new Bgpview(scanArgs));

    if(ui->moduleBinaryEdge->isChecked())
        this->startScanThread(new BinaryEdge(scanArgs));

    if(ui->moduleC99->isChecked())
        this->startScanThread(new C99(scanArgs));

    if(ui->moduleCircl->isChecked())
        this->startScanThread(new Circl(scanArgs));

    if(ui->moduleDnsbufferoverrun->isChecked())
        this->startScanThread(new Dnsbufferoverun(scanArgs));

    if(ui->moduleGithub->isChecked())
        this->startScanThread(new Github(scanArgs));

    if(ui->moduleHackerTargetFree->isChecked())
        this->startScanThread(new HackerTargetFree(scanArgs));

    if(ui->moduleHackerTargetPaid->isChecked())
        this->startScanThread(new HackerTargetPaid(scanArgs));

    if(ui->moduleMnemonicFree->isChecked())
        this->startScanThread(new MnemonicFree(scanArgs));

    if(ui->moduleMnemonicPaid->isChecked())
        this->startScanThread(new MnemonicPaid(scanArgs));

    if(ui->moduleOmnisint->isChecked())
        this->startScanThread(new Omnisint(scanArgs));

    if(ui->moduleOtxFree->isChecked())
        this->startScanThread(new OtxFree(scanArgs));

    if(ui->moduleOtxPaid->isChecked())
        this->startScanThread(new OtxPaid(scanArgs));

    if(ui->moduleProjectdiscovery->isChecked())
        this->startScanThread(new Projectdiscovery(scanArgs));

    if(ui->moduleRiskIq->isChecked())
        this->startScanThread(new RiskIq(scanArgs));

    if(ui->moduleRobtexFree->isChecked())
        this->startScanThread(new RobtexFree(scanArgs));

    if(ui->moduleRobtexPaid->isChecked())
        this->startScanThread(new RobtexPaid(scanArgs));

    if(ui->moduleSecuritytrails->isChecked())
        this->startScanThread(new SecurityTrails(scanArgs));

    if(ui->moduleShodan->isChecked())
        this->startScanThread(new Shodan(scanArgs));

    if(ui->moduleSpyse->isChecked())
        this->startScanThread(new Spyse(scanArgs));

    if(ui->moduleSublist3r->isChecked())
        this->startScanThread(new Sublist3r(scanArgs));

    if(ui->moduleThreatBook->isChecked())
        this->startScanThread(new ThreatBook(scanArgs));

    if(ui->moduleThreatcrowd->isChecked())
        this->startScanThread(new Threatcrowd(scanArgs));

    if(ui->moduleThreatminer->isChecked())
        this->startScanThread(new Threatminer(scanArgs));

    if(ui->moduleUrlscan->isChecked())
        this->startScanThread(new Urlscan(scanArgs));

    if(ui->moduleViewDns->isChecked())
        this->startScanThread(new ViewDns(scanArgs));

    if(ui->moduleVirusTotal->isChecked())
        this->startScanThread(new VirusTotal(scanArgs));

    if(ui->moduleWebResolver->isChecked())
        this->startScanThread(new WebResolver(scanArgs));

    if(ui->moduleWhoisXmlApi->isChecked())
        this->startScanThread(new WhoisXmlApi(scanArgs));

    if(ui->moduleZetalytics->isChecked())
        this->startScanThread(new ZETAlytics(scanArgs));

    if(ui->moduleZoomeye->isChecked())
        this->startScanThread(new ZoomEye(scanArgs));

    if(ui->moduleIpInfo->isChecked())
        this->startScanThread(new IpInfo(scanArgs));

    if(ui->moduleDnslytics->isChecked())
        this->startScanThread(new Dnslytics(scanArgs));

    if(ui->moduleDomainTools->isChecked())
        this->startScanThread(new DomainTools(scanArgs));

    if(ui->moduleMaltiverseFree->isChecked())
        this->startScanThread(new Maltiverse(scanArgs));

    if(ui->moduleN45HT->isChecked())
        this->startScanThread(new N45HT(scanArgs));

    if(ui->moduleOnyphe->isChecked())
        this->startScanThread(new Onyphe(scanArgs));

    if(ui->moduleRipe->isChecked())
        this->startScanThread(new Ripe(scanArgs));

    if(ui->moduleFullHunt->isChecked())
        this->startScanThread(new FullHunt(scanArgs));

    if(ui->moduleNetworksDB->isChecked())
        this->startScanThread(new NetworksDB(scanArgs));

    if(ui->moduleSpyOnWeb->isChecked())
        this->startScanThread(new SpyOnWeb(scanArgs));

    /****************************************************************************
                                 ARCHIVES
    *****************************************************************************/

    if(ui->moduleArchiveit->isChecked())
        this->startScanThread(new ArchiveIt(scanArgs));

    if(ui->moduleArchiveToday->isChecked())
        this->startScanThread(new ArchiveToday(scanArgs));

    if(ui->moduleArquivo->isChecked())
        this->startScanThread(new Arquivo(scanArgs));

    if(ui->moduleCommonCrawl->isChecked())
        this->startScanThread(new CommonCrawl(scanArgs));

    if(ui->moduleUKWebArchive->isChecked())
        this->startScanThread(new UKWebArchive(scanArgs));

    if(ui->moduleWaybackmachine->isChecked())
        this->startScanThread(new Waybackmachine(scanArgs));

    /****************************************************************************
                                 CERTS
    *****************************************************************************/

    if(ui->moduleCensys->isChecked())
        this->startScanThread(new Censys(scanArgs));

    if(ui->moduleCensysFree->isChecked())
        this->startScanThread(new CensysFree(scanArgs));

    if(ui->moduleCertspotter->isChecked())
        this->startScanThread(new Certspotter(scanArgs));

    if(ui->moduleCertspotterFree->isChecked())
        this->startScanThread(new CertspotterFree(scanArgs));

    if(ui->moduleCrtsh->isChecked())
        this->startScanThread(new Crtsh(scanArgs));

    if(ui->moduleGoogleCert->isChecked())
        this->startScanThread(new GoogleCert(scanArgs));

    /****************************************************************************
                                 EMAIL
    *****************************************************************************/

    if(ui->moduleHunter->isChecked())
        this->startScanThread(new Hunter(scanArgs));

    if(ui->moduleEmailCrawlr->isChecked())
        this->startScanThread(new EmailCrawlr(scanArgs));

    /****************************************************************************
                                 SITES
    *****************************************************************************/

    if(ui->moduleDnsdumpster->isChecked())
        this->startScanThread(new Dnsdumpster(scanArgs));

    if(ui->moduleNetcraft->isChecked())
        this->startScanThread(new Netcraft(scanArgs));

    if(ui->modulePagesInventory->isChecked())
        this->startScanThread(new PagesInventory(scanArgs));

    if(ui->modulePkey->isChecked())
        this->startScanThread(new Pkey(scanArgs));

    if(ui->moduleRapiddns->isChecked())
        this->startScanThread(new Rapiddns(scanArgs));

    if(ui->moduleSitedossier->isChecked())
        this->startScanThread(new SiteDossier(scanArgs));

    /****************************************************************************
                                 SCRAPE
    *****************************************************************************/

    if(ui->moduleAsk->isChecked())
        this->startScanThread(new Ask(scanArgs));

    if(ui->moduleBaidu->isChecked())
        this->startScanThread(new Baidu(scanArgs));

    if(ui->moduleDogpile->isChecked())
        this->startScanThread(new DogPile(scanArgs));

    if(ui->moduleDuckduckgo->isChecked())
        this->startScanThread(new DuckDuckGo(scanArgs));

    if(ui->moduleExalead->isChecked())
        this->startScanThread(new Exalead(scanArgs));

    if(ui->moduleYahoo->isChecked())
        this->startScanThread(new Yahoo(scanArgs));

    if(ui->moduleBing->isChecked())
        this->startScanThread(new Bing(scanArgs));

    /* after starting all choosen enumerations... */
    if(status->osint->activeThreads)
    {
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
        //...
        this->m_infoLog("------------------ start --------------");
    }
    else{
        QMessageBox::warning(this, "Error!", "Please Choose Engine For Enumeration!");
    }
}

void Osint::startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread(this);
    module->startScan(cThread);
    module->moveToThread(cThread);

    switch (ui->comboBoxOutput->currentIndex()) {
    case OUTPUT_SUBDOMAIN:
        connect(module, &AbstractOsintModule::subdomain, this, &Osint::onResultSubdomain);
        connect(module, &AbstractOsintModule::CNAME, this, &Osint::onResultCNAME);
        connect(module, &AbstractOsintModule::NS, this, &Osint::onResultNS);
        connect(module, &AbstractOsintModule::MX, this, &Osint::onResultMX);
        break;
    case OUTPUT_IP:
        connect(module, &AbstractOsintModule::ip, this, &Osint::onResultIp);
        connect(module, &AbstractOsintModule::ipA, this, &Osint::onResultA);
        connect(module, &AbstractOsintModule::ipAAAA, this, &Osint::onResultAAAA);
        break;
    case OUTPUT_SUBDOMAINIP:
        connect(module, &AbstractOsintModule::subdomainIp, this, &Osint::onResultSubdomainIp);
        break;
    case OUTPUT_EMAIL:
        connect(module, &AbstractOsintModule::email, this, &Osint::onResultEmail);
        break;
    case OUT_URL:
        connect(module, &AbstractOsintModule::url, this, &Osint::onResultUrl);
        break;
    case OUTPUT_ASN:
        connect(module, &AbstractOsintModule::asn, this, &Osint::onResultAsn);
        break;
    case OUTPUT_CIDR:
        connect(module, &AbstractOsintModule::cidr, this, &Osint::onResultCidr);
        break;
    case OUTPUT_SSLCERT:
        connect(module, &AbstractOsintModule::sslCert, this, &Osint::onResultSSLCert);
        break;
    }
    connect(module, &AbstractOsintModule::rateLimitLog, this, &Osint::onRateLimitLog);
    connect(module, &AbstractOsintModule::errorLog, this, &Osint::onErrorLog);
    connect(module, &AbstractOsintModule::infoLog, this, &Osint::onInfoLog);
    /* ... */
    connect(this, &Osint::stopScanThread, module, &AbstractOsintModule::onStop);
    connect(this, &Osint::pauseScanThread, module, &AbstractOsintModule::onPause);
    /* ... */
    connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

    cThread->start();
    status->osint->activeThreads++;
}
