/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : starts the OSINT scanning modules each in own thread
*/

#include "Osint.h"
#include "ui_Osint.h"

#include "src/modules/passive/OsintHeaders.h"


void Osint::onScanThreadEnded(){
    /* check if no active thread */
    status->activeScanThreads--;
    if(status->activeScanThreads)
        return;

    /* set the progress bar to 100% just in case */
    if(!status->isStopped)
        ui->progressBar->setValue(ui->progressBar->maximum());

    status->isNotActive = true;
    status->isPaused = false;
    status->isStopped = false;
    status->isRunning = false;

    /* reanabling the widgets... */
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);

    log("------------------ End ----------------");
    qInfo() << "[OSINT] Scan Ended";
}

void Osint::startScan(){
    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();
    m_scanArgs->targets.clear();
    m_scanArgs->input_Domain = false;
    m_scanArgs->input_IP = false;
    m_scanArgs->input_Email = false;
    m_scanArgs->input_URL = false;
    m_scanArgs->input_ASN = false;
    m_scanArgs->input_SSL = false;
    m_scanArgs->input_CIDR = false;
    m_scanArgs->input_Search = false;
    m_scanArgs->output_HostnameIP = false;
    m_scanArgs->output_Hostname = false;
    m_scanArgs->output_IP = false;
    m_scanArgs->output_Email = false;
    m_scanArgs->output_URL = false;
    m_scanArgs->output_ASN = false;
    m_scanArgs->output_SSL = false;
    m_scanArgs->output_CIDR = false;
    total_modules = 0;

    /* get the targets... */
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, ui->targets->getlistModel()->stringList())
            m_scanArgs->targets.enqueue(target);
    }else
        m_scanArgs->targets.enqueue(ui->lineEditTarget->text());

    /* getting input type as specified by user */
    switch(ui->comboBoxInput->currentIndex()){
    case INPUT::HOSTNAME:
        m_scanArgs->input_Domain = true;
        break;
    case INPUT::IP:
        m_scanArgs->input_IP = true;
        break;
    case INPUT::EMAIL:
        m_scanArgs->input_Email = true;
        break;
    case INPUT::URL:
        m_scanArgs->input_URL = true;
        break;
    case INPUT::ASN:
        m_scanArgs->input_ASN = true;
        break;
    case INPUT::CERT:
        m_scanArgs->input_SSL = true;
        break;
    case INPUT::CIDR:
        m_scanArgs->input_CIDR = true;
        break;
    case INPUT::QUERY_TERM:
        m_scanArgs->input_Search = true;
        break;
    }

    /* getting output type as specified by users */
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        m_scanArgs->output_HostnameIP = true;
        break;
    case osint::OUTPUT::SUBDOMAIN:
        m_scanArgs->output_Hostname = true;
        break;
    case osint::OUTPUT::IP:
        m_scanArgs->output_IP = true;
        break;
    case osint::OUTPUT::EMAIL:
        m_scanArgs->output_Email = true;
        break;
    case osint::OUTPUT::URL:
        m_scanArgs->output_URL = true;
        break;
    case osint::OUTPUT::ASN:
        m_scanArgs->output_ASN = true;
        break;
    case osint::OUTPUT::CERT:
        m_scanArgs->output_SSL = true;
        break;
    case osint::OUTPUT::CIDR:
        m_scanArgs->output_CIDR = true;
        break;
    }

    ///
    /// APIs...
    ///
    if(ui->moduleAnubis->isChecked())
        this->startScanThread(new Anubis(*m_scanArgs));

    if(ui->moduleASRank->isChecked())
        this->startScanThread(new ASRank(*m_scanArgs));

    if(ui->moduleBgpview->isChecked())
        this->startScanThread(new Bgpview(*m_scanArgs));

    if(ui->moduleBinaryEdge->isChecked())
        this->startScanThread(new BinaryEdge(*m_scanArgs));

    if(ui->moduleC99->isChecked())
        this->startScanThread(new C99(*m_scanArgs));

    if(ui->moduleCircl->isChecked())
        this->startScanThread(new Circl(*m_scanArgs));

    if(ui->moduleCirclPublic->isChecked())
        this->startScanThread(new CirclPublic(*m_scanArgs));

    if(ui->moduleDnsbufferoverrun->isChecked())
        this->startScanThread(new Dnsbufferoverun(*m_scanArgs));

    if(ui->moduleGithub->isChecked())
        this->startScanThread(new Github(*m_scanArgs));

    if(ui->moduleHackerTargetFree->isChecked())
        this->startScanThread(new HackerTargetFree(*m_scanArgs));

    if(ui->moduleHackerTargetPaid->isChecked())
        this->startScanThread(new HackerTarget(*m_scanArgs));

    if(ui->moduleMnemonicFree->isChecked())
        this->startScanThread(new MnemonicFree(*m_scanArgs));

    if(ui->moduleMnemonicPaid->isChecked())
        this->startScanThread(new Mnemonic(*m_scanArgs));

    if(ui->moduleOmnisint->isChecked())
        this->startScanThread(new Omnisint(*m_scanArgs));

    if(ui->moduleOtxFree->isChecked())
        this->startScanThread(new OtxFree(*m_scanArgs));

    if(ui->moduleOtxPaid->isChecked())
        this->startScanThread(new Otx(*m_scanArgs));

    if(ui->moduleProjectdiscovery->isChecked())
        this->startScanThread(new Projectdiscovery(*m_scanArgs));

    if(ui->moduleRiskIq->isChecked())
        this->startScanThread(new RiskIq(*m_scanArgs));

    if(ui->moduleRobtexFree->isChecked())
        this->startScanThread(new RobtexFree(*m_scanArgs));

    if(ui->moduleRobtexPaid->isChecked())
        this->startScanThread(new Robtex(*m_scanArgs));

    if(ui->moduleSecuritytrails->isChecked())
        this->startScanThread(new SecurityTrails(*m_scanArgs));

    if(ui->moduleShodan->isChecked())
        this->startScanThread(new Shodan(*m_scanArgs));

    if(ui->moduleSpyse->isChecked())
        this->startScanThread(new Spyse(*m_scanArgs));

    if(ui->moduleSublist3r->isChecked())
        this->startScanThread(new Sublist3r(*m_scanArgs));

    if(ui->moduleThreatBook->isChecked())
        this->startScanThread(new ThreatBook(*m_scanArgs));

    if(ui->moduleThreatcrowd->isChecked())
        this->startScanThread(new Threatcrowd(*m_scanArgs));

    if(ui->moduleThreatminer->isChecked())
        this->startScanThread(new Threatminer(*m_scanArgs));

    if(ui->moduleUrlscan->isChecked())
        this->startScanThread(new Urlscan(*m_scanArgs));

    if(ui->moduleViewDns->isChecked())
        this->startScanThread(new ViewDns(*m_scanArgs));

    if(ui->moduleVirusTotal->isChecked())
        this->startScanThread(new VirusTotal(*m_scanArgs));

    if(ui->moduleWebResolver->isChecked())
        this->startScanThread(new WebResolver(*m_scanArgs));

    if(ui->moduleWhoisXmlApi->isChecked())
        this->startScanThread(new WhoisXmlApi(*m_scanArgs));

    if(ui->moduleZetalytics->isChecked())
        this->startScanThread(new ZETAlytics(*m_scanArgs));

    if(ui->moduleZoomeye->isChecked())
        this->startScanThread(new ZoomEye(*m_scanArgs));

    if(ui->moduleIpInfo->isChecked())
        this->startScanThread(new IpInfo(*m_scanArgs));

    if(ui->moduleDnslytics->isChecked())
        this->startScanThread(new Dnslytics(*m_scanArgs));

    if(ui->moduleDomainTools->isChecked())
        this->startScanThread(new DomainTools(*m_scanArgs));

    if(ui->moduleMaltiverseFree->isChecked())
        this->startScanThread(new Maltiverse(*m_scanArgs));

    if(ui->moduleN45HT->isChecked())
        this->startScanThread(new N45HT(*m_scanArgs));

    if(ui->moduleOnyphe->isChecked())
        this->startScanThread(new Onyphe(*m_scanArgs));

    if(ui->moduleRipe->isChecked())
        this->startScanThread(new Ripe(*m_scanArgs));

    if(ui->moduleFullHunt->isChecked())
        this->startScanThread(new FullHunt(*m_scanArgs));

    if(ui->moduleNetworksDB->isChecked())
        this->startScanThread(new NetworksDB(*m_scanArgs));

    if(ui->moduleSpyOnWeb->isChecked())
        this->startScanThread(new SpyOnWeb(*m_scanArgs));

    if(ui->moduleLeakIX->isChecked())
        this->startScanThread(new LeakIX(*m_scanArgs));

    if(ui->modulePassiveTotal->isChecked())
        this->startScanThread(new PassiveTotal(*m_scanArgs));

    ///
    /// archives...
    ///
    if(ui->moduleArchiveit->isChecked())
        this->startScanThread(new ArchiveIt(*m_scanArgs));

    if(ui->moduleArchiveToday->isChecked())
        this->startScanThread(new ArchiveToday(*m_scanArgs));

    if(ui->moduleArquivo->isChecked())
        this->startScanThread(new Arquivo(*m_scanArgs));

    if(ui->moduleCommonCrawl->isChecked())
        this->startScanThread(new CommonCrawl(*m_scanArgs));

    if(ui->moduleUKWebArchive->isChecked())
        this->startScanThread(new UKWebArchive(*m_scanArgs));

    if(ui->moduleWaybackmachine->isChecked())
        this->startScanThread(new Waybackmachine(*m_scanArgs));

    ///
    /// SSL Certs...
    ///
    if(ui->moduleCensys->isChecked())
        this->startScanThread(new Censys(*m_scanArgs));

    if(ui->moduleCensysFree->isChecked())
        this->startScanThread(new CensysFree(*m_scanArgs));

    if(ui->moduleCertspotter->isChecked())
        this->startScanThread(new Certspotter(*m_scanArgs));

    if(ui->moduleCertspotterFree->isChecked())
        this->startScanThread(new CertspotterFree(*m_scanArgs));

    if(ui->moduleCrtsh->isChecked())
        this->startScanThread(new Crtsh(*m_scanArgs));

    if(ui->moduleGoogleCert->isChecked())
        this->startScanThread(new GoogleCert(*m_scanArgs));

    ///
    /// Email...
    ///
    if(ui->moduleHunter->isChecked())
        this->startScanThread(new Hunter(*m_scanArgs));

    if(ui->moduleEmailCrawlr->isChecked())
        this->startScanThread(new EmailCrawlr(*m_scanArgs));

    ///
    /// sites...
    ///
    if(ui->moduleDnsdumpster->isChecked())
        this->startScanThread(new Dnsdumpster(*m_scanArgs));

    if(ui->moduleNetcraft->isChecked())
        this->startScanThread(new Netcraft(*m_scanArgs));

    if(ui->modulePagesInventory->isChecked())
        this->startScanThread(new PagesInventory(*m_scanArgs));

    if(ui->modulePkey->isChecked())
        this->startScanThread(new Pkey(*m_scanArgs));

    if(ui->moduleRapiddns->isChecked())
        this->startScanThread(new Rapiddns(*m_scanArgs));

    if(ui->moduleSitedossier->isChecked())
        this->startScanThread(new SiteDossier(*m_scanArgs));

    ///
    /// scrape...
    ///
    if(ui->moduleAsk->isChecked())
        this->startScanThread(new Ask(*m_scanArgs));

    if(ui->moduleBaidu->isChecked())
        this->startScanThread(new Baidu(*m_scanArgs));

    if(ui->moduleDogpile->isChecked())
        this->startScanThread(new DogPile(*m_scanArgs));

    if(ui->moduleDuckduckgo->isChecked())
        this->startScanThread(new DuckDuckGo(*m_scanArgs));

    if(ui->moduleExalead->isChecked())
        this->startScanThread(new Exalead(*m_scanArgs));

    if(ui->moduleYahoo->isChecked())
        this->startScanThread(new YahooSearch(*m_scanArgs));

    if(ui->moduleBing->isChecked())
        this->startScanThread(new BingSearch(*m_scanArgs));

    if(ui->moduleGoogleSearch->isChecked())
        this->startScanThread(new GoogleSearch(*m_scanArgs));

    ///
    /// ip...
    ///
    if(ui->moduleIpInfo->isChecked())
        this->startScanThread(new IpInfo(*m_scanArgs));
    if(ui->moduleIpfy->isChecked())
        this->startScanThread(new Ipfy(*m_scanArgs));
    if(ui->moduleIpData->isChecked())
        this->startScanThread(new IpData(*m_scanArgs));

    /* set progressBar */
    ui->progressBar->setMaximum(total_modules*m_scanArgs->targets.length());

    /* after starting all choosen enumerations... */
    if(status->activeScanThreads)
    {
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
        log("------------------ start --------------");
        qInfo() << "[OSINT] Scan Started";
    }
    else
        QMessageBox::warning(this, tr("Error!"), tr("Please Choose Engine For Enumeration!"));
}

void Osint::startScanThread(AbstractOsintModule *module){
    total_modules++;
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
        connect(module, &AbstractOsintModule::resultIP, this, &Osint::onResultIP);
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
        connect(module, &AbstractOsintModule::resultURL, this, &Osint::onResultURL);
        break;
    case osint::OUTPUT::ASN:
        connect(module, &AbstractOsintModule::resultASN, this, &Osint::onResultASN);
        break;
    case osint::OUTPUT::CIDR:
        connect(module, &AbstractOsintModule::resultCIDR, this, &Osint::onResultCIDR);
        break;
    case osint::OUTPUT::CERT:
        connect(module, &AbstractOsintModule::resultSSL, this, &Osint::onResultSSL);
        break;
    }
    connect(module, &AbstractOsintModule::scanProgress, ui->progressBar, &QProgressBar::setValue);
    connect(module, &AbstractOsintModule::scanLog, this, &Osint::onScanLog);
    connect(this, &Osint::stopScanThread, module, &AbstractOsintModule::onStop);
    connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

    cThread->start();
    status->activeScanThreads++;
}
