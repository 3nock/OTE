#include "SSLEnum.h"
#include "ui_SSLEnum.h"

#include "src/dialogs/FailedScansDialog.h"


void SSLEnum::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        if(status->isStopped)
            this->log("---------------- Stopped ------------\n");
        else
            this->log("------------------ End --------------\n");

        qInfo() << "[SSL-Enum] Scan Ended";

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

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &SSLEnum::onReScan);
            failedScansDialog->show();
        }
    }
}

void SSLEnum::startScan(){
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

    m_scanArgs->output_EnumSSL = true;

    QThread *cThread = new QThread;
    switch (ui->comboBoxModule->currentIndex()) {
    case 0: // crt.sh
        Crtsh *crtsh = new Crtsh(*m_scanArgs);
        crtsh->startScan(cThread);
        crtsh->moveToThread(cThread);
        connect(crtsh, &Crtsh::resultRawSSL, this, &SSLEnum::onResult);
        connect(crtsh, &Crtsh::scanLog, this, &SSLEnum::onScanLog);
        connect(this, &SSLEnum::stopScanThread, crtsh, &AbstractOsintModule::onStop);
        connect(cThread, &QThread::finished, this, &SSLEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
}

void SSLEnum::onReScan(QQueue<QString> targets){
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
