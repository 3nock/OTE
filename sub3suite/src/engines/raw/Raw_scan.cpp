/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"
#include <QMessageBox>

#include "src/dialogs/FailedScansDialog.h"


void Raw::onScanThreadEnded(){
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
    status->activeScanThreads = 0;

    /* reanabling the widgets... */
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);

    log("------------------ End ----------------");
    qInfo() << "[RAW] Scan Ended";

    /* launching the failed scans dialog if there were failed scans */
    if(!m_failedScans.isEmpty()){
        FailedScansDialog *failedScansDialog = new FailedScansDialog(this, m_failedScans);
        failedScansDialog->setAttribute(Qt::WA_DeleteOnClose, true);

        connect(failedScansDialog, &FailedScansDialog::reScan, this, &Raw::onReScan);
        failedScansDialog->show();
    }
}

void Raw::startScan(){
    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    m_failedScans.clear();

    /* setting status */
    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    m_scanArgs->output_Raw = true;
    m_scanArgs->raw_query_id = ui->comboBoxOptions->currentIndex();
    m_scanArgs->raw_query_name = ui->comboBoxOptions->currentText();
    ui->progressBar->setMaximum(m_scanArgs->targets.length());

    /* starting the appropriate scan... */
    if(ui->moduleCertspotter->isChecked())
        this->startScanThread(new Certspotter(*m_scanArgs));

    if(ui->moduleCertspotterFree->isChecked())
        this->startScanThread(new CertspotterFree(*m_scanArgs));

    if(ui->moduleOtx->isChecked())
        this->startScanThread(new OtxFree(*m_scanArgs));

    if(ui->moduleSublist3r->isChecked())
        this->startScanThread(new Sublist3r(*m_scanArgs));

    if(ui->moduleThreatMiner->isChecked())
        this->startScanThread(new Threatminer(*m_scanArgs));

    if(ui->moduleThreatCrowd->isChecked())
        this->startScanThread(new Threatcrowd(*m_scanArgs));

    if(ui->moduleHackerTargetFree->isChecked())
        this->startScanThread(new HackerTargetFree(*m_scanArgs));

    if(ui->moduleDnsBufferOverRun->isChecked())
        this->startScanThread(new Dnsbufferoverun(*m_scanArgs));

    if(ui->moduleAnubis->isChecked())
        this->startScanThread(new Anubis(*m_scanArgs));

    if(ui->moduleProjectDiscovery->isChecked())
        this->startScanThread(new Projectdiscovery(*m_scanArgs));

    if(ui->moduleSpyse->isChecked())
        this->startScanThread(new Spyse(*m_scanArgs));

    if(ui->moduleGoogleCert->isChecked())
        this->startScanThread(new GoogleCert(*m_scanArgs));

    if(ui->moduleOmnisint->isChecked())
        this->startScanThread(new Omnisint(*m_scanArgs));

    if(ui->moduleRobtexPaid->isChecked())
        this->startScanThread(new Robtex(*m_scanArgs));

    if(ui->moduleVirusTotal->isChecked())
        this->startScanThread(new VirusTotal(*m_scanArgs));

    if(ui->moduleUrlScan->isChecked())
        this->startScanThread(new Urlscan(*m_scanArgs));

    if(ui->moduleWaybackMachine->isChecked())
        this->startScanThread(new Waybackmachine(*m_scanArgs));

    if(ui->moduleArchiveIt->isChecked())
        this->startScanThread(new ArchiveIt(*m_scanArgs));

    if(ui->moduleBgpview->isChecked())
        this->startScanThread(new Bgpview(*m_scanArgs));

    if(ui->moduleBinaryEdge->isChecked())
        this->startScanThread(new BinaryEdge(*m_scanArgs));

    if(ui->moduleC99->isChecked())
        this->startScanThread(new C99(*m_scanArgs));

    if(ui->moduleGithub->isChecked())
        this->startScanThread(new Github(*m_scanArgs));

    if(ui->moduleHunter->isChecked())
        this->startScanThread(new Hunter(*m_scanArgs));

    if(ui->moduleIpInfo->isChecked())
        this->startScanThread(new IpInfo(*m_scanArgs));

    if(ui->moduleMnemonicFree->isChecked())
        this->startScanThread(new MnemonicFree(*m_scanArgs));

    if(ui->moduleRiskIq->isChecked())
        this->startScanThread(new RiskIq(*m_scanArgs));

    if(ui->moduleRobtex->isChecked())
        this->startScanThread(new RobtexFree(*m_scanArgs));

    if(ui->moduleSecurityTrails->isChecked())
        this->startScanThread(new SecurityTrails(*m_scanArgs));

    if(ui->moduleShodan->isChecked())
        this->startScanThread(new Shodan(*m_scanArgs));

    if(ui->moduleThreatBook->isChecked())
        this->startScanThread(new ThreatBook(*m_scanArgs));

    if(ui->moduleWhoisXmlApi->isChecked())
        this->startScanThread(new WhoisXmlApi(*m_scanArgs));

    if(ui->moduleZETALytics->isChecked())
        this->startScanThread(new ZETAlytics(*m_scanArgs));

    if(ui->moduleZoomEye->isChecked())
        this->startScanThread(new ZoomEye(*m_scanArgs));

    if(ui->moduleIpApi->isChecked())
        this->startScanThread(new IpApi(*m_scanArgs));

    if(ui->moduleViewDns->isChecked())
        this->startScanThread(new ViewDns(*m_scanArgs));

    if(ui->moduleWebResolver->isChecked())
        this->startScanThread(new WebResolver(*m_scanArgs));

    if(ui->moduleCircl->isChecked())
        this->startScanThread(new Circl(*m_scanArgs));

    if(ui->moduleHackerTarget->isChecked())
        this->startScanThread(new HackerTarget(*m_scanArgs));

    if(ui->moduleMnemonic->isChecked())
        this->startScanThread(new Mnemonic(*m_scanArgs));

    if(ui->moduleOtx->isChecked())
        this->startScanThread(new Otx(*m_scanArgs));

    if(ui->moduleArin->isChecked())
        this->startScanThread(new Arin(*m_scanArgs));

    if(ui->moduleBuiltWith->isChecked())
        this->startScanThread(new BuiltWith(*m_scanArgs));

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

    if(ui->moduleAfrinic->isChecked())
        this->startScanThread(new Afrinic(*m_scanArgs));

    if(ui->moduleApnic->isChecked())
        this->startScanThread(new Apnic(*m_scanArgs));

    if(ui->moduleFullHunt->isChecked())
        this->startScanThread(new FullHunt(*m_scanArgs));

    if(ui->moduleSpamHaus->isChecked())
        this->startScanThread(new SpamHaus(*m_scanArgs));

    if(ui->moduleDebounce->isChecked())
        this->startScanThread(new Debounce(*m_scanArgs));

    if(ui->moduleHaveIBeenPawned->isChecked())
        this->startScanThread(new HaveIBeenPawned(*m_scanArgs));

    if(ui->moduleHost->isChecked())
        this->startScanThread(new Host(*m_scanArgs));

    if(ui->moduleJsonWhois->isChecked())
        this->startScanThread(new JsonWhois(*m_scanArgs));

    if(ui->moduleLeakIX->isChecked())
        this->startScanThread(new LeakIX(*m_scanArgs));

    if(ui->moduleNetworksDB->isChecked())
        this->startScanThread(new NetworksDB(*m_scanArgs));

    if(ui->moduleNeutrinoApi->isChecked())
        this->startScanThread(new NeutrinoApi(*m_scanArgs));

    if(ui->moduleSeon->isChecked())
        this->startScanThread(new Seon(*m_scanArgs));

    if(ui->moduleSpyOnWeb->isChecked())
        this->startScanThread(new SpyOnWeb(*m_scanArgs));

    if(ui->moduleWhatcms->isChecked())
        this->startScanThread(new Whatcms(*m_scanArgs));

    if(ui->moduleWhoxy->isChecked())
        this->startScanThread(new Whoxy(*m_scanArgs));

    if(ui->moduleArquivo->isChecked())
        this->startScanThread(new Arquivo(*m_scanArgs));

    if(ui->moduleUKWebArchive->isChecked())
        this->startScanThread(new UKWebArchive(*m_scanArgs));

    if(ui->moduleCensys->isChecked())
        this->startScanThread(new Censys(*m_scanArgs));

    if(ui->moduleEmailCrawlr->isChecked())
        this->startScanThread(new EmailCrawlr(*m_scanArgs));

    if(ui->moduleEmailFormat->isChecked())
        this->startScanThread(new EmailFormat(*m_scanArgs));

    if(ui->moduleEmailRep->isChecked())
        this->startScanThread(new EmailRep(*m_scanArgs));

    if(ui->moduleSnov->isChecked())
        this->startScanThread(new Snov(*m_scanArgs));

    if(ui->moduleTruMail->isChecked())
        this->startScanThread(new TruMail(*m_scanArgs));

    if(ui->moduleIpData->isChecked())
        this->startScanThread(new IpData(*m_scanArgs));

    if(ui->moduleIpGeoLocation->isChecked())
        this->startScanThread(new IpGeoLocation(*m_scanArgs));

    if(ui->moduleIpRegistry->isChecked())
        this->startScanThread(new IpRegistry(*m_scanArgs));

    if(ui->moduleIpStack->isChecked())
        this->startScanThread(new IpStack(*m_scanArgs));

    if(ui->moduleAbuseIPDB->isChecked())
        this->startScanThread(new AbuseIPDB(*m_scanArgs));

    if(ui->moduleBotScout->isChecked())
        this->startScanThread(new BotScout(*m_scanArgs));

    if(ui->moduleFraudGuard->isChecked())
        this->startScanThread(new FraudGuard(*m_scanArgs));

    if(ui->moduleHybridAnalysis->isChecked())
        this->startScanThread(new HybridAnalysis(*m_scanArgs));

    if(ui->moduleIpQualityScore->isChecked())
        this->startScanThread(new IpQualityScore(*m_scanArgs));

    if(ui->moduleLeakLookup->isChecked())
        this->startScanThread(new LeakLookup(*m_scanArgs));

    if(ui->moduleWappalyzer->isChecked())
        this->startScanThread(new Wappalyzer(*m_scanArgs));

    if(ui->modulePassiveTotal->isChecked())
        this->startScanThread(new PassiveTotal(*m_scanArgs));

    if(ui->moduleIpfy->isChecked())
        this->startScanThread(new Ipfy(*m_scanArgs));

    if(ui->moduleCirclPublic->isChecked())
        this->startScanThread(new CirclPublic(*m_scanArgs));

    if(ui->moduleASRank->isChecked())
        this->startScanThread(new ASRank(*m_scanArgs));
}

void Raw::startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread(this);
    module->startScan(cThread);
    module->moveToThread(cThread);

    connect(module, &AbstractOsintModule::scanProgress, ui->progressBar, &QProgressBar::setValue);
    connect(module, &AbstractOsintModule::resultRawJSON, this, &Raw::onResults);
    connect(module, &AbstractOsintModule::resultRawTXT, this, &Raw::onResultsTxt);
    connect(module, &AbstractOsintModule::scanLog, this, &Raw::onScanLog);
    connect(this, &Raw::stopScanThread, module, &AbstractOsintModule::onStop);
    connect(cThread, &QThread::finished, this, &Raw::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

    cThread->start();
    status->activeScanThreads++;
}

void Raw::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* getting targets */
    m_scanArgs->targets = targets;

    /* starting the scan */
    this->startScan();

    /* after starting all choosen modules */
    if(status->activeScanThreads){
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
        log("------------------ start --------------");
        qInfo() << "[RAW] Re-Scan Started";
    }
    else
        QMessageBox::warning(this, tr("Error!"), tr("Please Choose Engine For Enumeration!"));
}
