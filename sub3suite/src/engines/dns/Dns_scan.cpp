/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"

#include <QTime>
#include "src/dialogs/FailedScansDialog.h"


void Dns::startScan(){
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setText(tr("Pause"));

    /* status */
    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    m_failedScans.clear();

    /* getting srv wordlist if checked &
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */
    if(ui->checkBoxSRV->isChecked()){
        m_scanArgs->srvWordlist = m_srvWordlitsModel->stringList();

        if(m_scanArgs->config->threads > m_scanArgs->srvWordlist.length())
            status->activeScanThreads = m_scanArgs->srvWordlist.length();
        else
            status->activeScanThreads = m_scanArgs->config->threads;

        /* set progressbar maximum value */
        ui->progressBar->setMaximum(m_scanArgs->targets.length()*m_scanArgs->srvWordlist.length());
    }
    else {
        if(m_scanArgs->config->threads > m_scanArgs->targets.length())
            status->activeScanThreads = m_scanArgs->targets.length();
        else
            status->activeScanThreads = m_scanArgs->config->threads;
        /* set progressbar maximum value */
        ui->progressBar->setMaximum(m_scanArgs->targets.length());
    }

    /* renewing scan statistics */
    m_scanStats->failed = 0;
    m_scanStats->resolved = 0;
    m_scanStats->threads = status->activeScanThreads;
    m_scanStats->targets = m_scanArgs->targets.length();
    m_scanStats->nameservers = m_scanArgs->config->nameservers.length();

    /* getting the arguments for Dns Records Scan... */
    m_scanArgs->currentSRV = 0;
    m_scanArgs->progress = 0;
    m_scanArgs->RecordType_a = ui->checkBoxA->isChecked();
    m_scanArgs->RecordType_aaaa = ui->checkBoxAAAA->isChecked();
    m_scanArgs->RecordType_mx = ui->checkBoxMX->isChecked();
    m_scanArgs->RecordType_ns = ui->checkBoxNS->isChecked();
    m_scanArgs->RecordType_txt = ui->checkBoxTXT->isChecked();
    m_scanArgs->RecordType_cname = ui->checkBoxCNAME->isChecked();
    m_scanArgs->RecordType_srv = ui->checkBoxSRV->isChecked();
    m_scanArgs->RecordType_any = ui->checkBoxANY->isChecked();

    /* start timer */
    m_timer.start();

    /* get first target */
    if(m_scanArgs->RecordType_srv)
        m_scanArgs->currentTarget = m_scanArgs->targets.dequeue();

    /* loop to create threads for scan... */
    for(int i = 0; i < status->activeScanThreads; i++)
    {
        dns::Scanner *scanner = new dns::Scanner(m_scanArgs);
        QThread *cThread = new QThread(this);
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);

        if(m_scanArgs->RecordType_srv)
            connect(scanner, &dns::Scanner::scanResult, this, &Dns::onScanResult_srv);
        else
            connect(scanner, &dns::Scanner::scanResult, this, &Dns::onScanResult);
        connect(scanner, &dns::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &dns::Scanner::scanLog, this, &Dns::onScanLog);
        connect(cThread, &QThread::finished, this, &Dns::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &QThread::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Dns::stopScanThread, scanner, &dns::Scanner::onStopScan);
        connect(this, &Dns::pauseScanThread, scanner, &dns::Scanner::onPauseScan);
        connect(this, &Dns::resumeScanThread, scanner, &dns::Scanner::onResumeScan, Qt::DirectConnection);

        cThread->start();
    }
    status->isRunning = true;
}

void Dns::onReScan(QQueue<QString> targets){
    if(targets.isEmpty())
        return;

    /* clear */
    m_scanArgs->targets.clear();
    m_scanArgs->srvWordlist.clear();

    /* get targets */
    m_scanArgs->targets = targets;

    /* start scan */
    this->startScan();

    /* logs */
    log("----------------- Re-Scan ---------------\n");
    qInfo() << "[DNS] Re-Scan Started";
}

void Dns::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        /* display the scan summary on logs */
        this->scanSummary();

        if(status->isStopped)
            log("---------------- Stopped ------------\n");
        else
            log("------------------ End --------------\n");

        qInfo() << "[DNS] Scan Ended";

        /* set the progress bar to 100% just in case... */
        if(!status->isStopped)
            ui->progressBar->setValue(ui->progressBar->maximum());

        m_scanArgs->targets.clear();

        status->isNotActive = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isRunning = false;

        ui->buttonStart->setText("Start");
        ui->buttonStop->setDisabled(true);

        // launching the failed scans dialog if there were failed scans
        if(!m_failedScans.isEmpty()){
            FailedScansDialog *failedScansDialog = new FailedScansDialog(this, m_failedScans);
            failedScansDialog->setAttribute(Qt::WA_DeleteOnClose, true);

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &Dns::onReScan);
            failedScansDialog->show();
        }
    }
}

void Dns::scanSummary(){
    /* elapsed time */
    QTime time = QTime::fromMSecsSinceStartOfDay(m_timer.elapsed());

    /* write to log file */
    ui->plainTextEditLogs->appendHtml("<font color=\"white\">  [ Scan Summary ]</font>");
    ui->plainTextEditLogs->appendHtml("[ Resolved ]    : <font color=\"green\">"+QString::number(m_scanStats->resolved)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Failed ]      : <font color=\"red\">"+QString::number(m_scanStats->failed)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Threads ]     : <font color=\"green\">"+QString::number(m_scanStats->threads)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Targets ]     : <font color=\"green\">"+QString::number(m_scanStats->targets)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Nameservers ] : <font color=\"green\">"+QString::number(m_scanStats->nameservers)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Time (hh:mm:ss:zzz) ]        : <font color=\"green\">"+time.toString("hh:mm:ss:zzz")+"</font>");
}
