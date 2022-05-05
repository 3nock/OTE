/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IPTool.h"
#include "ui_IPTool.h"

#include <QTime>
#include <QThread>
#include "src/dialogs/FailedScansDialog.h"


void IPTool::startScan(){
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
    m_scanStats->nameservers = m_scanArgs->config->nameservers.length();

    /* set progressbar maximum value */
    ui->progressBar->setMaximum(m_scanArgs->targets.length());
    m_scanArgs->progress = 0;

    /* start timer */
    m_timer.start();

    switch(ui->comboBoxOption->currentIndex()){
    case 1: // PORT SCAN
    {
        m_portscannerArgs->target_ports.clear();
        m_portscannerArgs->target_ips.clear();
        m_portscannerArgs->progress = 0;

        if(ui->radioButtonDefault->isChecked())
            m_portscannerArgs->target_ports << 80 << 443 << 21 << 990 << 22 << 25 << 465 << 587 << 2525 ;

        if(ui->radioButtonCustom->isChecked()){
            foreach(const QString &port, ui->lineEditCustom->text().split(","))
                m_portscannerArgs->target_ports << port.toUShort();
        }
        if(ui->radioButtonRange->isChecked()){
            ushort from = ui->lineEditFrom->text().toUShort();
            ushort to = ui->lineEditTo->text().toUShort();
            for(ushort i = from; i < to; i++)
                m_portscannerArgs->target_ports << i;
        }
        m_portscannerArgs->timeout = m_scanArgs->config->timeout;
        m_portscannerArgs->target_ips = m_scanArgs->targets;
    }
        break;

    case 2: // PING SCAN
    {
        m_pingscannerArgs->targets = m_scanArgs->targets;
        m_pingscannerArgs->progress = 0;
        m_pingscannerArgs->timeout = m_scanArgs->config->timeout;
    }
    }

    /* loop to create threads for enumeration... */
    for(int i = 0; i < status->activeScanThreads; i++)
    {
        switch (ui->comboBoxOption->currentIndex()) {
        case 0: // ACTIVE DNS
        {
            ip::Scanner *scanner = new ip::Scanner(m_scanArgs);
            QThread *cThread = new QThread;
            scanner->startScan(cThread);
            scanner->moveToThread(cThread);
            connect(scanner, &ip::Scanner::scanResult, this, &IPTool::onScanResult_dns);
            connect(scanner, &ip::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
            connect(scanner, &ip::Scanner::scanLog, this, &IPTool::onScanLog);
            connect(cThread, &QThread::finished, this, &IPTool::onScanThreadEnded);
            connect(cThread, &QThread::finished, scanner, &ip::Scanner::deleteLater);
            connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
            connect(this, &IPTool::stopScanThread, scanner, &ip::Scanner::onStopScan);
            connect(this, &IPTool::pauseScanThread, scanner, &ip::Scanner::onPauseScan);
            connect(this, &IPTool::resumeScanThread, scanner, &ip::Scanner::onResumeScan, Qt::DirectConnection);
            cThread->start();
            break;
        }
        case 1: // ACTIVE PORT
        {
            port::Scanner *scanner = new port::Scanner(m_portscannerArgs);
            QThread *cThread = new QThread;
            scanner->startScan(cThread);
            scanner->moveToThread(cThread);
            connect(scanner, &port::Scanner::scanResult_ip, this, &IPTool::onScanResult_port);
            connect(scanner, &port::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
            connect(scanner, &port::Scanner::scanLog, this, &IPTool::onScanLog);
            connect(cThread, &QThread::finished, this, &IPTool::onScanThreadEnded);
            connect(cThread, &QThread::finished, scanner, &port::Scanner::deleteLater);
            connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
            connect(this, &IPTool::stopScanThread, scanner, &port::Scanner::onStopScan, Qt::DirectConnection);
            connect(this, &IPTool::pauseScanThread, scanner, &port::Scanner::onPauseScan, Qt::DirectConnection);
            connect(this, &IPTool::resumeScanThread, scanner, &port::Scanner::onResumeScan, Qt::DirectConnection);
            cThread->start();
            break;
        }
        case 2: // ACTIVE PING
        {
            ping::Scanner *scanner = new ping::Scanner(m_pingscannerArgs);
            QThread *cThread = new QThread;
            scanner->startScan(cThread);
            scanner->moveToThread(cThread);
            connect(scanner, &ping::Scanner::scanResult_ip, this, &IPTool::onScanResult_ping);
            connect(scanner, &ping::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
            connect(scanner, &ping::Scanner::scanLog, this, &IPTool::onScanLog);
            connect(cThread, &QThread::finished, this, &IPTool::onScanThreadEnded);
            connect(cThread, &QThread::finished, scanner, &ping::Scanner::deleteLater);
            connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
            connect(this, &IPTool::stopScanThread, scanner, &ping::Scanner::onStopScan, Qt::DirectConnection);
            connect(this, &IPTool::pauseScanThread, scanner, &ping::Scanner::onPauseScan, Qt::DirectConnection);
            connect(this, &IPTool::resumeScanThread, scanner, &ping::Scanner::onResumeScan, Qt::DirectConnection);
            cThread->start();
        }
        }
    }
    status->isRunning = true;
}

void IPTool::onReScan(QQueue<QString> targets){
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
    qInfo() << "[IP] Re-Scan Started";
}

void IPTool::onScanThreadEnded(){
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

        qInfo() << "[IP] Scan Ended";

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

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &IPTool::onReScan);
            failedScansDialog->show();
        }
    }
}

void IPTool::scanSummary(){
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
