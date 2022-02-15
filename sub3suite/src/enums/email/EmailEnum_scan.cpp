#include "EmailEnum.h"
#include "ui_EmailEnum.h"

#include "src/modules/passive/OsintModulesHeaders.h"


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

void EmailEnum::startScan(){
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
    scanArgs.config->progress = 0;

    scanArgs.outputInfoEmail = true;

    QThread *cThread = new QThread;
    switch (ui->comboBoxEngine->currentIndex()) {
    case 0: // EmailRep
        EmailRep *emailRep = new EmailRep(scanArgs);
        emailRep->startScan(cThread);
        emailRep->moveToThread(cThread);
        connect(emailRep, &EmailRep::infoEmail, this, &EmailEnum::onResult);
        connect(emailRep, &EmailRep::infoLog, this, &EmailEnum::onInfoLog);
        connect(emailRep, &EmailRep::errorLog, this, &EmailEnum::onErrorLog);
        connect(emailRep, &EmailRep::rateLimitLog, this, &EmailEnum::onRateLimitLog);
        connect(cThread, &QThread::finished, this, &EmailEnum::onScanThreadEnded);
        connect(cThread, &QThread::finished, emailRep, &EmailRep::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
        status->activeScanThreads++;
    }
}
