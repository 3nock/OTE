#include "EmailEnum.h"
#include "ui_EmailEnum.h"

#include "src/modules/passive/OsintModulesHeaders.h"
#include "src/dialogs/FailedScansDialog.h"


void EmailEnum::onScanThreadEnded(){
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

        qInfo() << "[Email-Enum] Scan Ended";

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

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &EmailEnum::onReScan);
            failedScansDialog->show();
        }
    }
}

void EmailEnum::startScan(){
    /* resetting */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();
    m_failedScans.clear();

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

    m_scanArgs->output_EnumEmail = true;

    /* start scan thread */
    switch (ui->comboBoxEngine->currentIndex())
    {
    case 0: // TruMail
        this->startScanThread(new TruMail(*m_scanArgs));
        break;
    }
}

void EmailEnum::startScanThread(AbstractOsintModule *module){
    QThread *cThread = new QThread;
    module->startScan(cThread);
    module->moveToThread(cThread);
    connect(module, &AbstractOsintModule::resultEnumEmail, this, &EmailEnum::onResult);
    connect(this, &EmailEnum::stopScanThread, module, &AbstractOsintModule::onStop);
    connect(module, &AbstractOsintModule::scanProgress, ui->progressBar, &QProgressBar::setValue);
    connect(module, &AbstractOsintModule::scanLog, this, &EmailEnum::onScanLog);
    connect(cThread, &QThread::finished, this, &EmailEnum::onScanThreadEnded);
    connect(cThread, &QThread::finished, module, &AbstractOsintModule::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
    cThread->start();
    status->activeScanThreads++;
}

void EmailEnum::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* getting targets */
    m_scanArgs->targets = targets;

    /* start scan */
    this->startScan();

    /* logs */
    this->log("------------------ Re-Scan ----------------");
    qInfo() << "[Email-Enum] Re-Scan Started";
}
