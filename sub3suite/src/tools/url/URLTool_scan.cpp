/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "URLTool.h"
#include "ui_URLTool.h"

#include <QTime>
#include <QThread>
#include "src/dialogs/FailedScansDialog.h"


void URLTool::startScan(){
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setText("Pause");

    /* status */
    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    m_failedScans.clear();

    /*
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */
    if(m_scanArgs->config->threads > m_scanArgs->targets.length())
        status->activeScanThreads = m_scanArgs->targets.length();
    else
        status->activeScanThreads = m_scanArgs->config->threads;

    /* renewing scan statistics */
    m_scanStats->failed = 0;
    m_scanStats->resolved = 0;
    m_scanStats->threads = status->activeScanThreads;
    m_scanStats->targets = m_scanArgs->targets.length();

    /* set progressbar maximum value */
    ui->progressBar->setMaximum(m_scanArgs->targets.length());
    m_scanArgs->progress = 0;

    /* start timer */
    m_timer.start();

    /* loop to create threads for enumeration... */
    for(int i = 0; i < status->activeScanThreads; i++)
    {
        url::Scanner *scanner = new url::Scanner(m_scanArgs);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);

        connect(scanner, &url::Scanner::scanResult, this, &URLTool::onScanResult);
        connect(scanner, &url::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &url::Scanner::scanLog, this, &URLTool::onScanLog);
        connect(cThread, &QThread::finished, this, &URLTool::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &url::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &URLTool::stopScanThread, scanner, &url::Scanner::onStopScan);
        connect(this, &URLTool::pauseScanThread, scanner, &url::Scanner::onPauseScan);
        connect(this, &URLTool::resumeScanThread, scanner, &url::Scanner::onResumeScan, Qt::DirectConnection);

        cThread->start();
    }
    status->isRunning = true;
}

void URLTool::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* clear */
    m_scanArgs->targets.clear();

    /* get targets */
    m_scanArgs->targets = targets;

    /* start scan */
    this->startScan();

    /* logs */
    log("----------------- Re-Scan ---------------\n");
    qInfo() << "[URL] Re-Scan Started";
}

void URLTool::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        /* display the scan summary on logs */
        scanSummary();

        if(status->isStopped)
            log("---------------- Stopped ------------\n");
        else
            log("------------------ End --------------\n");

        qInfo() << "[URL] Scan Ended";

        /* set the progress bar to 100% just in case... */
        if(!status->isStopped)
            ui->progressBar->setValue(ui->progressBar->maximum());

        m_scanArgs->targets.clear();

        status->isNotActive = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isRunning = false;

        ui->buttonStart->setText(tr("Start"));
        ui->buttonStop->setDisabled(true);

        // launching the failed scans dialog if there were failed scans
        if(!m_failedScans.isEmpty()){
            FailedScansDialog *failedScansDialog = new FailedScansDialog(this, m_failedScans);
            failedScansDialog->setAttribute(Qt::WA_DeleteOnClose, true);

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &URLTool::onReScan);
            failedScansDialog->show();
        }
    }
}

void URLTool::scanSummary(){
    /* elapsed time */
    QTime time = QTime::fromMSecsSinceStartOfDay(m_timer.elapsed());

    /* write to log file */
    ui->plainTextEditLogs->appendHtml("<font color=\"white\">  [ Scan Summary ]</font>");
    ui->plainTextEditLogs->appendHtml("[ Resolved ]    : <font color=\"green\">"+QString::number(m_scanStats->resolved)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Failed ]      : <font color=\"red\">"+QString::number(m_scanStats->failed)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Threads ]     : <font color=\"green\">"+QString::number(m_scanStats->threads)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Targets ]     : <font color=\"green\">"+QString::number(m_scanStats->targets)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Time (hh:mm:ss:zzz) ]        : <font color=\"green\">"+time.toString("hh:mm:ss:zzz")+"</font>");
}
