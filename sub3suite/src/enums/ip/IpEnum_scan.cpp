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

    m_scanArgs->output_EnumIP = true;

    /* start scanthread */
    switch (ui->comboBoxOutput->currentIndex())
    {
    case 0: // IP informations
        switch (ui->comboBoxEngine->currentIndex())
        {
        case 0: // IPINFO
            this->startScanThread(new IpInfo(*m_scanArgs));
            break;
        case 1: // IPAPI
            this->startScanThread(new IpApi(*m_scanArgs));
            break;
        }
        break;

    case 1: // Rerverse IP-Address
        switch (ui->comboBoxEngine->currentIndex())
        {
        case 0: // DNSLytics
            this->startScanThread_reverseIP(new Dnslytics(*m_scanArgs));
            break;
        }
    }
}

void IpEnum::startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread;
    module->startScan(cThread);
    module->moveToThread(cThread);
    connect(module, &AbstractOsintModule::resultEnumIP, this, &IpEnum::onResult);
    connect(module, &AbstractOsintModule::scanLog, this, &IpEnum::onScanLog);
    connect(cThread, &QThread::finished, this, &IpEnum::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
    cThread->start();
    status->activeScanThreads++;
}

void IpEnum::startScanThread_reverseIP(AbstractOsintModule *module){
    QThread *cThread = new QThread;
    module->startScan(cThread);
    module->moveToThread(cThread);
    connect(module, &AbstractOsintModule::resultEnumIP, this, &IpEnum::onResult_reverse);
    connect(module, &AbstractOsintModule::scanLog, this, &IpEnum::onScanLog);
    connect(cThread, &QThread::finished, this, &IpEnum::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
    cThread->start();
    status->activeScanThreads++;
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
