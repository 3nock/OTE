#include "CidrEnum.h"
#include "ui_CidrEnum.h"


void CidrEnum::onScanThreadEnded(){
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
    }
}

void CidrEnum::startScan(){
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

    scanArgs.outputInfoCidr = true;

    switch (ui->comboBoxEngine->currentIndex()) {
    case 0: // Bgpview
        QThread *cThread = new QThread;
        Bgpview *bgpview = new Bgpview(scanArgs);
        bgpview->startScan(cThread);
        bgpview->moveToThread(cThread);
        connect(bgpview, &IpInfo::infoCIDR, this, &CidrEnum::onResult);
        connect(bgpview, &IpInfo::infoLog, this, &CidrEnum::onInfoLog);
        connect(bgpview, &IpInfo::errorLog, this, &CidrEnum::onErrorLog);
        connect(bgpview, &IpInfo::rateLimitLog, this, &CidrEnum::onRateLimitLog);
        connect(this, &CidrEnum::stopScanThread, bgpview, &AbstractOsintModule::onStop);
        connect(this, &CidrEnum::pauseScanThread, bgpview, &AbstractOsintModule::onPause);
        connect(cThread, &QThread::finished, this, &CidrEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
}
