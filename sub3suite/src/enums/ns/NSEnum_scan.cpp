#include "NSEnum.h"
#include "ui_NSEnum.h"

#include "src/dialogs/FailedScansDialog.h"


void NSEnum::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        if(status->isStopped)
            this->log("---------------- Stopped ------------\n");
        else
            this->log("------------------ End --------------\n");

        qInfo() << "[NS-Enum] Scan Ended";

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

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &NSEnum::onReScan);
            failedScansDialog->show();
        }
    }
}

void NSEnum::startScan(){
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

    m_scanArgs->output_EnumNS = true;

    /* start scanthread */
    switch (ui->comboBoxEngine->currentIndex())
    {
    case 0: // DNSLytics
        this->startScanThread(new Dnslytics(*m_scanArgs));
        break;

    case 1: // SpyOnWeb
        this->startScanThread(new SpyOnWeb(*m_scanArgs));
        break;

    case 2: // ViewDNS
        this->startScanThread(new ViewDns(*m_scanArgs));
        break;

    case 3: // WhoIsXMLAPI
        this->startScanThread(new WhoisXmlApi(*m_scanArgs));
        break;

    case 4: // DomainTools
        this->startScanThread(new DomainTools(*m_scanArgs));
        break;
    }
}

void NSEnum::startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread;
    module->startScan(cThread);
    module->moveToThread(cThread);
    connect(module, &AbstractOsintModule::resultEnumNS, this, &NSEnum::onResult);
    connect(module, &AbstractOsintModule::scanLog, this, &NSEnum::onScanLog);
    connect(cThread, &QThread::finished, this, &NSEnum::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
    cThread->start();
    status->activeScanThreads++;
}

void NSEnum::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* getting targets */
    m_scanArgs->targets = targets;

    /* start scan */
    this->startScan();

    /* logs */
    this->log("------------------ Re-Scan ----------------");
    qInfo() << "[NS-Enum] Re-Scan Started";
}
