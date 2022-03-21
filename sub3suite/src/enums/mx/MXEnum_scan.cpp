#include "MXEnum.h"
#include "ui_MXEnum.h"

#include "src/dialogs/FailedScansDialog.h"


void MXEnum::onScanThreadEnded(){
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

        qInfo() << "[MX-Enum] Scan Ended";

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

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &MXEnum::onReScan);
            failedScansDialog->show();
        }
    }
}

void MXEnum::startScan(){
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

    m_scanArgs->outputInfoMX = true;

    QThread *cThread = new QThread;
    switch (ui->comboBoxEngine->currentIndex()) {
    case 0: // DNSLytics
        Dnslytics *dnslytics = new Dnslytics(*m_scanArgs);
        dnslytics->startScan(cThread);
        dnslytics->moveToThread(cThread);
        connect(dnslytics, &Dnslytics::infoMX, this, &MXEnum::onResult);
        connect(dnslytics, &Dnslytics::infoLog, this, &MXEnum::onInfoLog);
        connect(dnslytics, &Dnslytics::errorLog, this, &MXEnum::onErrorLog);
        connect(cThread, &QThread::finished, this, &MXEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, dnslytics, &Dnslytics::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
}

void MXEnum::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* getting targets */
    m_scanArgs->targets = targets;

    /* start scan */
    this->startScan();

    /* logs */
    this->log("------------------ Re-Scan ----------------");
    qInfo() << "[MX-Enum] Re-Scan Started";
}
