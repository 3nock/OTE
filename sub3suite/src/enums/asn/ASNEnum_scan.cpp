#include "ASNEnum.h"
#include "ui_ASNEnum.h"


void ASNEnum::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        /* display the scan summary on logs
        m_scanSummary();
        */

        if(status->isStopped)
            this->log("---------------- Stopped ------------\n");
        else
            this->log("------------------ End --------------\n");

        /* set the progress bar to 100% just in case... */
        if(!status->isStopped)
            ui->progressBar->setValue(ui->progressBar->maximum());

        status->isNotActive = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isRunning = false;

        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setDisabled(true);

        /* launching the failed scans dialog if there were failed scans
        if(!m_failedScans.isEmpty()){
            FailedScansDialog *failedScansDialog = new FailedScansDialog(this, m_failedScans);
            failedScansDialog->setAttribute(Qt::WA_DeleteOnClose, true);

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &Active::onReScan);
            failedScansDialog->show();
        }
        */
    }
}

void ASNEnum::startScan(){
    ScanArgs scanArgs;

    /* resetting */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();

    /* getting targets */
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, m_targetsListModel->stringList())
            scanArgs.targets.enqueue(target);
    }else{
        scanArgs.targets.enqueue(ui->lineEditTarget->text());
    }

    /* progressbar maximum value */
    ui->progressBar->setMaximum(scanArgs.targets.length());
    scanArgs.progress = 0;

    switch(ui->comboBoxOption->currentIndex()){
    case 0: // ASN INFO
        scanArgs.outputInfoAsn = true;
        scanArgs.outputInfoAsnPeers = false;
        scanArgs.outputInfoAsnPrefixes = false;
        break;
    case 1: // ASN PEERS
        scanArgs.outputInfoAsn = false;
        scanArgs.outputInfoAsnPeers = true;
        scanArgs.outputInfoAsnPrefixes = false;
        break;
    case 2: // ASN PREFIXES
        scanArgs.outputInfoAsn = false;
        scanArgs.outputInfoAsnPeers = false;
        scanArgs.outputInfoAsnPrefixes = true;
        break;
    }

    switch (ui->comboBoxEngine->currentIndex()) {
    case 0: // Bgpview
        QThread *cThread = new QThread;
        Bgpview *bgpview = new Bgpview(scanArgs);
        bgpview->startScan(cThread);
        bgpview->moveToThread(cThread);

        switch(ui->comboBoxOption->currentIndex()){
        case 0:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsn);
            break;
        case 1:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsnPeers);
            break;
        case 2:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsnPrefixes);
            break;
        }
        connect(bgpview, &Bgpview::infoLog, this, &ASNEnum::onInfoLog);
        connect(bgpview, &Bgpview::errorLog, this, &ASNEnum::onErrorLog);
        connect(bgpview, &Bgpview::rateLimitLog, this, &ASNEnum::onRateLimitLog);
        connect(bgpview, &Bgpview::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(this, &ASNEnum::stopScanThread, bgpview, &AbstractOsintModule::onStop);
        connect(cThread, &QThread::finished, this, &ASNEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
}
