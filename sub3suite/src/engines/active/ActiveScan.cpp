/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Active.h"
#include "ui_Active.h"
#include <QThread>


void Active::m_stopScan(){

}

void Active::m_startScan(){
    /*
      if the numner of threads is greater than the number of wordlists, set the
      number of threads to use to the number of wordlists available to avoid
      creating more threads than needed...
    */
    int wordlistCount = m_targetListModel->rowCount();
    int threadsCount = 0;
    if(threadsCount > wordlistCount)
        threadsCount = wordlistCount;

    status->activeScanThreads = threadsCount;

    /* loop to create threads for enumeration... */
    for(int i = 0; i < threadsCount; i++)
    {
        active::Scanner *scanner = new active::Scanner(m_scanArgs);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        connect(scanner, &active::Scanner::scanResult, this, &Active::onScanResult);
        connect(scanner, &active::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &active::Scanner::scanLog, this, &Active::onScanLog);
        connect(cThread, &QThread::finished, this, &Active::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &active::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Active::stopScanThread, scanner, &active::Scanner::onStopScan);
        cThread->start();
    }
    status->isRunning = true;
}

void Active::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if subdomainIp Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        if(status->isPaused){
            status->isRunning = false;
            return;
        }
        else{
            /* set the progress bar to 100% just in case...*/
            if(!status->isStopped)
                ui->progressBar->setValue(ui->progressBar->maximum());

            status->isPaused = false;
            status->isStopped = false;
            status->isRunning = false;

            /* enabling and disabling widgets */
            ui->buttonStart->setEnabled(true);
            ui->buttonStop->setDisabled(true);
        }
    }
}
