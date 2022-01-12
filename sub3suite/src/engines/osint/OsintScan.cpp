/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Osint.h"
#include "ui_Osint.h"

#include "src/modules/passive/OsintModulesHeaders.h"


void Osint::m_startScan(){
    ScanArgs scanArgs;

    /* get the targets... */
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, ui->targets->getlistModel()->stringList())
            scanArgs.targets.push(target);
    }else
        scanArgs.targets.push(ui->lineEditTarget->text());

    /* getting input type as specified by user... */
    switch(ui->comboBoxInput->currentIndex()){
    case INPUT::HOSTNAME:
        scanArgs.inputDomain = true;
        break;
    case INPUT::IP:
        scanArgs.inputIp = true;
        break;
    case INPUT::EMAIL:
        scanArgs.inputEmail = true;
        break;
    case INPUT::URL:
        scanArgs.inputUrl = true;
        break;
    case INPUT::ASN:
        scanArgs.inputAsn = true;
        break;
    case INPUT::CERT:
        scanArgs.inputSSLCert = true;
        break;
    case INPUT::CIDR:
        scanArgs.inputCidr = true;
        break;
    }

    /* getting output type as specified by user... */
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        scanArgs.outputSubdomainIp = true;
        break;
    case osint::OUTPUT::SUBDOMAIN:
        scanArgs.outputSubdomain = true;
        break;
    case osint::OUTPUT::IP:
        scanArgs.outputIp = true;
        break;
    case osint::OUTPUT::EMAIL:
        scanArgs.outputEmail = true;
        break;
    case osint::OUTPUT::URL:
        scanArgs.outputUrl = true;
        break;
    case osint::OUTPUT::ASN:
        scanArgs.outputAsn = true;
        break;
    case osint::OUTPUT::CERT:
        scanArgs.outputSSLCert = true;
        break;
    case osint::OUTPUT::CIDR:
        scanArgs.outputCidr = true;
        break;
    }

    /****************************************************************************
                                    APIs
    *****************************************************************************/

    if(ui->moduleAnubis->isChecked())
        this->m_startScanThread(new Anubis(scanArgs));

    if(ui->moduleBgpview->isChecked())
        this->m_startScanThread(new Bgpview(scanArgs));

    if(ui->moduleBinaryEdge->isChecked())
        this->m_startScanThread(new BinaryEdge(scanArgs));

    if(ui->moduleC99->isChecked())
        this->m_startScanThread(new C99(scanArgs));

    if(ui->moduleCircl->isChecked())
        this->m_startScanThread(new Circl(scanArgs));

    if(ui->moduleDnsbufferoverrun->isChecked())
        this->m_startScanThread(new Dnsbufferoverun(scanArgs));

    if(ui->moduleGithub->isChecked())
        this->m_startScanThread(new Github(scanArgs));

    if(ui->moduleHackerTargetFree->isChecked())
        this->m_startScanThread(new HackerTargetFree(scanArgs));

    if(ui->moduleHackerTargetPaid->isChecked())
        this->m_startScanThread(new HackerTargetPaid(scanArgs));

    if(ui->moduleMnemonicFree->isChecked())
        this->m_startScanThread(new MnemonicFree(scanArgs));

    if(ui->moduleMnemonicPaid->isChecked())
        this->m_startScanThread(new MnemonicPaid(scanArgs));

    if(ui->moduleOmnisint->isChecked())
        this->m_startScanThread(new Omnisint(scanArgs));

    if(ui->moduleOtxFree->isChecked())
        this->m_startScanThread(new OtxFree(scanArgs));

    if(ui->moduleOtxPaid->isChecked())
        this->m_startScanThread(new OtxPaid(scanArgs));

    if(ui->moduleProjectdiscovery->isChecked())
        this->m_startScanThread(new Projectdiscovery(scanArgs));

    if(ui->moduleRiskIq->isChecked())
        this->m_startScanThread(new RiskIq(scanArgs));

    if(ui->moduleRobtexFree->isChecked())
        this->m_startScanThread(new RobtexFree(scanArgs));

    if(ui->moduleRobtexPaid->isChecked())
        this->m_startScanThread(new RobtexPaid(scanArgs));

    if(ui->moduleSecuritytrails->isChecked())
        this->m_startScanThread(new SecurityTrails(scanArgs));

    if(ui->moduleShodan->isChecked())
        this->m_startScanThread(new Shodan(scanArgs));

    if(ui->moduleSpyse->isChecked())
        this->m_startScanThread(new Spyse(scanArgs));

    if(ui->moduleSublist3r->isChecked())
        this->m_startScanThread(new Sublist3r(scanArgs));

    if(ui->moduleThreatBook->isChecked())
        this->m_startScanThread(new ThreatBook(scanArgs));

    if(ui->moduleThreatcrowd->isChecked())
        this->m_startScanThread(new Threatcrowd(scanArgs));

    if(ui->moduleThreatminer->isChecked())
        this->m_startScanThread(new Threatminer(scanArgs));

    if(ui->moduleUrlscan->isChecked())
        this->m_startScanThread(new Urlscan(scanArgs));

    if(ui->moduleViewDns->isChecked())
        this->m_startScanThread(new ViewDns(scanArgs));

    if(ui->moduleVirusTotal->isChecked())
        this->m_startScanThread(new VirusTotal(scanArgs));

    if(ui->moduleWebResolver->isChecked())
        this->m_startScanThread(new WebResolver(scanArgs));

    if(ui->moduleWhoisXmlApi->isChecked())
        this->m_startScanThread(new WhoisXmlApi(scanArgs));

    if(ui->moduleZetalytics->isChecked())
        this->m_startScanThread(new ZETAlytics(scanArgs));

    if(ui->moduleZoomeye->isChecked())
        this->m_startScanThread(new ZoomEye(scanArgs));

    if(ui->moduleIpInfo->isChecked())
        this->m_startScanThread(new IpInfo(scanArgs));

    if(ui->moduleDnslytics->isChecked())
        this->m_startScanThread(new Dnslytics(scanArgs));

    if(ui->moduleDomainTools->isChecked())
        this->m_startScanThread(new DomainTools(scanArgs));

    if(ui->moduleMaltiverseFree->isChecked())
        this->m_startScanThread(new Maltiverse(scanArgs));

    if(ui->moduleN45HT->isChecked())
        this->m_startScanThread(new N45HT(scanArgs));

    if(ui->moduleOnyphe->isChecked())
        this->m_startScanThread(new Onyphe(scanArgs));

    if(ui->moduleRipe->isChecked())
        this->m_startScanThread(new Ripe(scanArgs));

    if(ui->moduleFullHunt->isChecked())
        this->m_startScanThread(new FullHunt(scanArgs));

    if(ui->moduleNetworksDB->isChecked())
        this->m_startScanThread(new NetworksDB(scanArgs));

    if(ui->moduleSpyOnWeb->isChecked())
        this->m_startScanThread(new SpyOnWeb(scanArgs));

    /****************************************************************************
                                 ARCHIVES
    *****************************************************************************/

    if(ui->moduleArchiveit->isChecked())
        this->m_startScanThread(new ArchiveIt(scanArgs));

    if(ui->moduleArchiveToday->isChecked())
        this->m_startScanThread(new ArchiveToday(scanArgs));

    if(ui->moduleArquivo->isChecked())
        this->m_startScanThread(new Arquivo(scanArgs));

    if(ui->moduleCommonCrawl->isChecked())
        this->m_startScanThread(new CommonCrawl(scanArgs));

    if(ui->moduleUKWebArchive->isChecked())
        this->m_startScanThread(new UKWebArchive(scanArgs));

    if(ui->moduleWaybackmachine->isChecked())
        this->m_startScanThread(new Waybackmachine(scanArgs));

    /****************************************************************************
                                 CERTS
    *****************************************************************************/

    if(ui->moduleCensys->isChecked())
        this->m_startScanThread(new Censys(scanArgs));

    if(ui->moduleCensysFree->isChecked())
        this->m_startScanThread(new CensysFree(scanArgs));

    if(ui->moduleCertspotter->isChecked())
        this->m_startScanThread(new Certspotter(scanArgs));

    if(ui->moduleCertspotterFree->isChecked())
        this->m_startScanThread(new CertspotterFree(scanArgs));

    if(ui->moduleCrtsh->isChecked())
        this->m_startScanThread(new Crtsh(scanArgs));

    if(ui->moduleGoogleCert->isChecked())
        this->m_startScanThread(new GoogleCert(scanArgs));

    /****************************************************************************
                                 EMAIL
    *****************************************************************************/

    if(ui->moduleHunter->isChecked())
        this->m_startScanThread(new Hunter(scanArgs));

    if(ui->moduleEmailCrawlr->isChecked())
        this->m_startScanThread(new EmailCrawlr(scanArgs));

    /****************************************************************************
                                 SITES
    *****************************************************************************/

    if(ui->moduleDnsdumpster->isChecked())
        this->m_startScanThread(new Dnsdumpster(scanArgs));

    if(ui->moduleNetcraft->isChecked())
        this->m_startScanThread(new Netcraft(scanArgs));

    if(ui->modulePagesInventory->isChecked())
        this->m_startScanThread(new PagesInventory(scanArgs));

    if(ui->modulePkey->isChecked())
        this->m_startScanThread(new Pkey(scanArgs));

    if(ui->moduleRapiddns->isChecked())
        this->m_startScanThread(new Rapiddns(scanArgs));

    if(ui->moduleSitedossier->isChecked())
        this->m_startScanThread(new SiteDossier(scanArgs));

    /****************************************************************************
                                 SCRAPE
    *****************************************************************************/

    if(ui->moduleAsk->isChecked())
        this->m_startScanThread(new Ask(scanArgs));

    if(ui->moduleBaidu->isChecked())
        this->m_startScanThread(new Baidu(scanArgs));

    if(ui->moduleDogpile->isChecked())
        this->m_startScanThread(new DogPile(scanArgs));

    if(ui->moduleDuckduckgo->isChecked())
        this->m_startScanThread(new DuckDuckGo(scanArgs));

    if(ui->moduleExalead->isChecked())
        this->m_startScanThread(new Exalead(scanArgs));

    if(ui->moduleYahoo->isChecked())
        this->m_startScanThread(new Yahoo(scanArgs));

    if(ui->moduleBing->isChecked())
        this->m_startScanThread(new Bing(scanArgs));

    /* after starting all choosen enumerations... */
    if(status->activeScanThreads)
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

void Osint::m_startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread(this);
    module->startScan(cThread);
    module->moveToThread(cThread);

    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::SUBDOMAIN:
        connect(module, &AbstractOsintModule::resultSubdomain, this, &Osint::onResultSubdomain);
        connect(module, &AbstractOsintModule::resultCNAME, this, &Osint::onResultCNAME);
        connect(module, &AbstractOsintModule::resultNS, this, &Osint::onResultNS);
        connect(module, &AbstractOsintModule::resultMX, this, &Osint::onResultMX);
        break;
    case osint::OUTPUT::IP:
        connect(module, &AbstractOsintModule::resultIp, this, &Osint::onResultIp);
        connect(module, &AbstractOsintModule::resultA, this, &Osint::onResultA);
        connect(module, &AbstractOsintModule::resultAAAA, this, &Osint::onResultAAAA);
        break;
    case osint::OUTPUT::SUBDOMAINIP:
        connect(module, &AbstractOsintModule::resultSubdomainIp, this, &Osint::onResultSubdomainIp);
        break;
    case osint::OUTPUT::EMAIL:
        connect(module, &AbstractOsintModule::resultEmail, this, &Osint::onResultEmail);
        break;
    case OUT_URL:
        connect(module, &AbstractOsintModule::resultUrl, this, &Osint::onResultUrl);
        break;
    case osint::OUTPUT::ASN:
        connect(module, &AbstractOsintModule::resultASN, this, &Osint::onResultAsn);
        break;
    case osint::OUTPUT::CIDR:
        connect(module, &AbstractOsintModule::resultCidr, this, &Osint::onResultCidr);
        break;
    case osint::OUTPUT::CERT:
        connect(module, &AbstractOsintModule::resultSSL, this, &Osint::onResultSSLCert);
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
    status->activeScanThreads++;
}
