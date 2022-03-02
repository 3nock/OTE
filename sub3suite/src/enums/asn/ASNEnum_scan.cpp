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

    /* getting targets */
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, m_targetsListModel->stringList())
            m_scanArgs->targets.enqueue(target);
    }else{
        m_scanArgs->targets.enqueue(ui->lineEditTarget->text());
    }

    /* progressbar maximum value */
    ui->progressBar->setMaximum(m_scanArgs->targets.length());
    m_scanArgs->config->progress = 0;

    switch(ui->comboBoxOption->currentIndex()){
    case 0: // ASN INFO
        m_scanArgs->outputInfoAsn = true;
        m_scanArgs->outputInfoAsnPeers = false;
        m_scanArgs->outputInfoAsnPrefixes = false;
        break;
    case 1: // ASN PEERS
        m_scanArgs->outputInfoAsn = false;
        m_scanArgs->outputInfoAsnPeers = true;
        m_scanArgs->outputInfoAsnPrefixes = false;
        break;
    case 2: // ASN PREFIXES
        m_scanArgs->outputInfoAsn = false;
        m_scanArgs->outputInfoAsnPeers = false;
        m_scanArgs->outputInfoAsnPrefixes = true;
        break;
    }

    switch (ui->comboBoxEngine->currentIndex()) {
    case 0: // Bgpview
        QThread *cThread = new QThread;
        Bgpview *bgpview = new Bgpview(*m_scanArgs);
        bgpview->startScan(cThread);
        bgpview->moveToThread(cThread);

        switch(ui->comboBoxOption->currentIndex()){
        case 0:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsn);
            break;
        case 1:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsnPeers);
            break;
        case 2:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsnPrefixes);
            break;
        }
        connect(bgpview, &Bgpview::infoLog, this, &ASNEnum::onInfoLog);
        connect(bgpview, &Bgpview::errorLog, this, &ASNEnum::onErrorLog);
        connect(bgpview, &Bgpview::rateLimitLog, this, &ASNEnum::onRateLimitLog);
        connect(bgpview, &Bgpview::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(this, &ASNEnum::stopScanThread, bgpview, &AbstractOsintModule::onStop);
        connect(cThread, &QThread::finished, this, &ASNEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
}

void ASNEnum::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    /* resetting */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();

    m_scanArgs->targets = targets;

    /* progressbar maximum value */
    ui->progressBar->setMaximum(m_scanArgs->targets.length());
    m_scanArgs->config->progress = 0;

    switch(ui->comboBoxOption->currentIndex()){
    case 0: // ASN INFO
        m_scanArgs->outputInfoAsn = true;
        m_scanArgs->outputInfoAsnPeers = false;
        m_scanArgs->outputInfoAsnPrefixes = false;
        break;
    case 1: // ASN PEERS
        m_scanArgs->outputInfoAsn = false;
        m_scanArgs->outputInfoAsnPeers = true;
        m_scanArgs->outputInfoAsnPrefixes = false;
        break;
    case 2: // ASN PREFIXES
        m_scanArgs->outputInfoAsn = false;
        m_scanArgs->outputInfoAsnPeers = false;
        m_scanArgs->outputInfoAsnPrefixes = true;
        break;
    }

    switch (ui->comboBoxEngine->currentIndex()) {
    case 0: // Bgpview
        QThread *cThread = new QThread;
        Bgpview *bgpview = new Bgpview(*m_scanArgs);
        bgpview->startScan(cThread);
        bgpview->moveToThread(cThread);

        switch(ui->comboBoxOption->currentIndex()){
        case 0:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsn);
            break;
        case 1:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsnPeers);
            break;
        case 2:
            connect(bgpview, &Bgpview::infoASN, this, &ASNEnum::onResultsAsnPrefixes);
            break;
        }
        connect(bgpview, &Bgpview::infoLog, this, &ASNEnum::onInfoLog);
        connect(bgpview, &Bgpview::errorLog, this, &ASNEnum::onErrorLog);
        connect(bgpview, &Bgpview::rateLimitLog, this, &ASNEnum::onRateLimitLog);
        connect(bgpview, &Bgpview::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(this, &ASNEnum::stopScanThread, bgpview, &AbstractOsintModule::onStop);
        connect(cThread, &QThread::finished, this, &ASNEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }

    /* logs */
    this->log("------------------ Re-Scan ----------------");
    qInfo() << "[ASN-Enum] Re-Scan Started";
}
