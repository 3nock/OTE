/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Ssl.h"
#include "ui_Ssl.h"

#include "src/dialogs/FailedScansDialog.h"
#include "src/modules/active/SSLScanner.h"


void Ssl::m_startScan(){
    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();

    m_failedScans.clear();
    m_scanArgs->targets.clear();

    /* get targets */
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, m_targetListModel->stringList())
            m_scanArgs->targets.enqueue(target);
    }else {
        m_scanArgs->targets.enqueue(ui->lineEditTarget->text());
    }

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

    switch (ui->comboBoxOutput->currentIndex()) {
        case 0: // subdomain
            m_scanArgs->output = ssl::OUTPUT::SUBDOMAIN;
            break;
        case 1: // cert id
            switch (ui->comboBoxOption->currentIndex()) {
                case 0: // SHA1
                    m_scanArgs->output = ssl::OUTPUT::SHA1;
                    break;
                case 1: // SHA256
                    m_scanArgs->output = ssl::OUTPUT::SHA256;
                    break;
            }
            break;
        case 2: // cert raw
            m_scanArgs->output = ssl::OUTPUT::RAW;
            break;
    }

    switch (ui->comboBoxProtocal->currentIndex()) {
    case 0: // HTTPS
        m_scanArgs->port = ssl::PORT::HTTPS;
        break;
    case 1: // FTP
        m_scanArgs->port = ssl::PORT::FTP;
        break;
    case 2: // FTPs
        m_scanArgs->port = ssl::PORT::FTPs;
        break;
    case 3: // SSH
        m_scanArgs->port = ssl::PORT::SSH;
        break;
    }

    /* loop to create threads for enumeration... */
    for(int i = 0; i < status->activeScanThreads; i++)
    {
        ssl::Scanner *scanner = new ssl::Scanner(m_scanArgs);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);

        /* results signals & slots... */
        switch (ui->comboBoxOutput->currentIndex()) {
        case 0: // subdomain
            connect(scanner, &ssl::Scanner::resultSubdomain, this, &Ssl::onScanResultSubdomain);
            break;
        case 1: // cert id
            switch (ui->comboBoxOption->currentIndex()) {
            case 0: // SHA1
                connect(scanner, &ssl::Scanner::resultSHA1, this, &Ssl::onScanResultSHA1);
                break;
            case 1: // SHA256
                connect(scanner, &ssl::Scanner::resultSHA256, this, &Ssl::onScanResultSHA256);
                break;
            }
            break;
        case 2: // raw cert
            connect(scanner, &ssl::Scanner::resultRaw, this, &Ssl::onScanResultRaw);
            break;
        }
        connect(scanner, &ssl::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &ssl::Scanner::scanLog, this, &Ssl::onScanLog);
        connect(cThread, &QThread::finished, this, &Ssl::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &ssl::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Ssl::stopScanThread, scanner, &ssl::Scanner::onStopScan);
        connect(this, &Ssl::pauseScanThread, scanner, &ssl::Scanner::onPauseScan);
        connect(this, &Ssl::resumeScanThread, scanner, &ssl::Scanner::onResumeScan, Qt::DirectConnection);
        cThread->start();
    }
    status->isRunning = true;
}

void Ssl::onReScan(QQueue<QString> targets){
    /* checks */
    if(targets.isEmpty())
        return;

    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setText("Pause");

    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    /* logs */
    m_log("----------------- Re-Scan ---------------\n");
    qInfo() << "Scan Started";

    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();

    m_failedScans.clear();
    m_scanArgs->targets.clear();

    m_scanArgs->targets = targets;

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

    switch (ui->comboBoxOutput->currentIndex()) {
        case 0: // subdomain
            m_scanArgs->output = ssl::OUTPUT::SUBDOMAIN;
            break;
        case 1: // cert id
            switch (ui->comboBoxOption->currentIndex()) {
                case 0: // SHA1
                    m_scanArgs->output = ssl::OUTPUT::SHA1;
                    break;
                case 1: // SHA256
                    m_scanArgs->output = ssl::OUTPUT::SHA256;
                    break;
            }
            break;
        case 2: // cert raw
            m_scanArgs->output = ssl::OUTPUT::RAW;
            break;
    }

    switch (ui->comboBoxProtocal->currentIndex()) {
    case 0: // HTTPS
        m_scanArgs->port = ssl::PORT::HTTPS;
        break;
    case 1: // FTP
        m_scanArgs->port = ssl::PORT::FTP;
        break;
    case 2: // FTPs
        m_scanArgs->port = ssl::PORT::FTPs;
        break;
    case 3: // SSH
        m_scanArgs->port = ssl::PORT::SSH;
        break;
    }

    /* loop to create threads for enumeration... */
    for(int i = 0; i < status->activeScanThreads; i++)
    {
        ssl::Scanner *scanner = new ssl::Scanner(m_scanArgs);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);

        /* results signals & slots... */
        switch (ui->comboBoxOutput->currentIndex()) {
        case 0: // subdomain
            connect(scanner, &ssl::Scanner::resultSubdomain, this, &Ssl::onScanResultSubdomain);
            break;
        case 1: // cert id
            switch (ui->comboBoxOption->currentIndex()) {
            case 0: // SHA1
                connect(scanner, &ssl::Scanner::resultSHA1, this, &Ssl::onScanResultSHA1);
                break;
            case 1: // SHA256
                connect(scanner, &ssl::Scanner::resultSHA256, this, &Ssl::onScanResultSHA256);
                break;
            }
            break;
        case 2: // raw cert
            connect(scanner, &ssl::Scanner::resultRaw, this, &Ssl::onScanResultRaw);
            break;
        }
        connect(scanner, &ssl::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &ssl::Scanner::scanLog, this, &Ssl::onScanLog);
        connect(cThread, &QThread::finished, this, &Ssl::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &ssl::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Ssl::stopScanThread, scanner, &ssl::Scanner::onStopScan);
        connect(this, &Ssl::pauseScanThread, scanner, &ssl::Scanner::onPauseScan);
        connect(this, &Ssl::resumeScanThread, scanner, &ssl::Scanner::onResumeScan, Qt::DirectConnection);
        cThread->start();
    }

    status->isRunning = true;
}

void Ssl::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        /* display the scan summary on logs */
        m_scanSummary();

        if(status->isStopped)
            m_log("---------------- Stopped ------------\n");
        else
            m_log("------------------ End --------------\n");

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

            connect(failedScansDialog, &FailedScansDialog::reScan, this, &Ssl::onReScan);
            failedScansDialog->show();
        }
    }
}

void Ssl::m_scanSummary(){
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
