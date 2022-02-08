#include "SSLEnum.h"
#include "ui_SSLEnum.h"


void SSLEnum::onScanThreadEnded(){
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

void SSLEnum::startScan(){
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

    scanArgs.outputInfoSSLCert = true;

    QThread *cThread = new QThread;
    switch (ui->comboBoxModule->currentIndex()) {
    case 0: // crt.sh
        Crtsh *crtsh = new Crtsh(scanArgs);
        crtsh->startScan(cThread);
        crtsh->moveToThread(cThread);
        connect(crtsh, &Crtsh::rawCert, this, &SSLEnum::onResult);
        connect(crtsh, &Crtsh::infoLog, this, &SSLEnum::onInfoLog);
        connect(crtsh, &Crtsh::errorLog, this, &SSLEnum::onErrorLog);
        connect(crtsh, &Crtsh::rateLimitLog, this, &SSLEnum::onRateLimitLog);
        connect(this, &SSLEnum::stopScanThread, crtsh, &AbstractOsintModule::onStop);
        connect(cThread, &QThread::finished, this, &SSLEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
}
