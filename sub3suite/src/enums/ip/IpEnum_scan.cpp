#include "IpEnum.h"
#include "ui_IpEnum.h"

#include "src/dialogs/FailedScansDialog.h"


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

        qInfo() << "[IP-Enum] Scan Ended";

        /* set the progress bar to 100% just in case... */
        if(!status->isStopped)
            ui->progressBar->setValue(ui->progressBar->maximum());

        status->isNotActive = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isRunning = false;

        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setDisabled(true);

        /* launching the failed scans dialog if there were failed scans */
        if(!m_failedScans.isEmpty()){
            FailedScansDialog *failedScansDialog = new FailedScansDialog(this, m_failedScans);
            failedScansDialog->setAttribute(Qt::WA_DeleteOnClose, true);

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &IpEnum::onReScan);
            failedScansDialog->show();
        }
    }
}

void IpEnum::startScan(){
    /* resetting */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();

    /* enabling/disabling widgets... */
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    /* setting status */
    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    /* progressbar maximum value */
    ui->progressBar->setMaximum(m_scanArgs->targets.length());
    m_scanArgs->config->progress = 0;

    m_scanArgs->outputInfoIp = true;

    QThread *cThread = new QThread;
    switch (ui->comboBoxOutput->currentIndex())
    {
    case 0: // IP informations
        switch (ui->comboBoxEngine->currentIndex()) {
        case 0: // IPINFO
        {
            IpInfo *ipinfo = new IpInfo(*m_scanArgs);
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
            IpApi *ipApi = new IpApi(*m_scanArgs);
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
        break;

    case 1: // Rerverse IP-Address
        switch (ui->comboBoxEngine->currentIndex()) {
        case 0: // DNSLytics
        {
            Dnslytics *dnslytics = new Dnslytics(*m_scanArgs);
            dnslytics->startScan(cThread);
            dnslytics->moveToThread(cThread);
            connect(dnslytics, &Dnslytics::infoIp, this, &IpEnum::onResult_reverse);
            connect(dnslytics, &Dnslytics::infoLog, this, &IpEnum::onInfoLog);
            connect(dnslytics, &Dnslytics::errorLog, this, &IpEnum::onErrorLog);
            connect(dnslytics, &Dnslytics::rateLimitLog, this, &IpEnum::onRateLimitLog);
            connect(this, &IpEnum::stopScanThread, dnslytics, &Dnslytics::onStop);
            connect(cThread, &QThread::finished, this, &IpEnum::onScanThreadEnded);
            connect(cThread, &QThread::finished, dnslytics, &Dnslytics::deleteLater);
            connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
            cThread->start();
            status->activeScanThreads++;
        }
            break;
        }
        break;
    }

}

void IpEnum::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* getting targets */
    m_scanArgs->targets = targets;

    /* start scan */
    this->startScan();

    /* logs */
    this->log("------------------ Re-Scan ----------------");
    qInfo() << "[IP-Enum] Re-Scan Started";
}
