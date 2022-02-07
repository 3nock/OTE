#include "IpEnum.h"
#include "ui_IpEnum.h"


void IpEnum::onScanThreadEnded(){
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

void IpEnum::startScan(){
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

    QThread *cThread = new QThread;
    switch (ui->comboBoxEngine->currentIndex()) {
    case 0: // IPINFO
    {
        IpInfo *ipinfo = new IpInfo(scanArgs);
        ipinfo->startScan(cThread);
        ipinfo->moveToThread(cThread);
        connect(ipinfo, &IpInfo::infoIp, this, &IpEnum::onResult);
        connect(ipinfo, &IpInfo::infoLog, this, &IpEnum::onInfoLog);
        connect(ipinfo, &IpInfo::errorLog, this, &IpEnum::onErrorLog);
        connect(ipinfo, &IpInfo::rateLimitLog, this, &IpEnum::onRateLimitLog);
        connect(this, &IpEnum::stopScanThread, ipinfo, &IpInfo::onStop);
        connect(cThread, &QThread::finished, this, &IpEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
        break;
    case 1: // IPAPI
    {
        IpApi *ipApi = new IpApi(scanArgs);
        ipApi->startScan(cThread);
        ipApi->moveToThread(cThread);
        connect(ipApi, &IpApi::infoIp, this, &IpEnum::onResult);
        connect(ipApi, &IpApi::infoLog, this, &IpEnum::onInfoLog);
        connect(ipApi, &IpApi::errorLog, this, &IpEnum::onErrorLog);
        connect(ipApi, &IpApi::rateLimitLog, this, &IpEnum::onRateLimitLog);
        connect(this, &IpEnum::stopScanThread, ipApi, &IpApi::onStop);
        connect(cThread, &QThread::finished, this, &IpEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, ipApi, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
    }
}
