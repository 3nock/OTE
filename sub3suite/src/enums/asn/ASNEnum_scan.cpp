#include "ASNEnum.h"
#include "ui_ASNEnum.h"

#include "src/dialogs/FailedScansDialog.h"


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

        qInfo() << "[ASN-Enum] Scan Ended";

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

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &ASNEnum::onReScan);
            failedScansDialog->show();
        }
    }
}

void ASNEnum::startScan(){
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

    switch(ui->comboBoxOption->currentIndex()){
    case 0: // ASN INFO
        m_scanArgs->output_EnumASN = true;
        m_scanArgs->output_EnumASNPeers = false;
        m_scanArgs->output_EnumASNPrefixes = false;
        break;
    case 1: // ASN PEERS
        m_scanArgs->output_EnumASN = false;
        m_scanArgs->output_EnumASNPeers = true;
        m_scanArgs->output_EnumASNPrefixes = false;
        break;
    case 2: // ASN PREFIXES
        m_scanArgs->output_EnumASN = false;
        m_scanArgs->output_EnumASNPeers = false;
        m_scanArgs->output_EnumASNPrefixes = true;
        break;
    }

    /* starting scan thread */
    switch (ui->comboBoxEngine->currentIndex())
    {
    case 0: // Bgpview
        this->startScanThread(new Bgpview(*m_scanArgs));
        break;
    }
}

void ASNEnum::startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread;
    module->startScan(cThread);
    module->moveToThread(cThread);

    switch(ui->comboBoxOption->currentIndex()){
    case 0:
        connect(module, &AbstractOsintModule::resultEnumASN, this, &ASNEnum::onResultsAsn);
        break;
    case 1:
        connect(module, &AbstractOsintModule::resultEnumASN, this, &ASNEnum::onResultsAsnPeers);
        break;
    case 2:
        connect(module, &AbstractOsintModule::resultEnumASN, this, &ASNEnum::onResultsAsnPrefixes);
        break;
    }
    connect(module, &AbstractOsintModule::scanLog, this, &ASNEnum::onScanLog);
    connect(cThread, &QThread::finished, this, &ASNEnum::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
    cThread->start();
    status->activeScanThreads++;
}

void ASNEnum::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* getting targets */
    m_scanArgs->targets = targets;

    /* start scan */
    this->startScan();

    /* logs */
    this->log("------------------ Re-Scan ----------------");
    qInfo() << "[ASN-Enum] Re-Scan Started";
}
