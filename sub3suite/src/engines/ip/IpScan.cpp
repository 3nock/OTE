#include "Ip.h"
#include "ui_Ip.h"


void Ip::m_stopScan(){

}

void Ip::m_pauseScan(){
    /*
     if the scan was already paused, then this current click is to
     Resume the scan, just csubdomainIp the startScan, with the same arguments and
     it will continue at where it ended...
    */
    if(status->ip->isPaused){
        status->ip->isPaused = false;
        this->m_startScan();
    }
    else{
        status->ip->isPaused = true;
        emit stopScanThread();
    }
}

void Ip::m_resumeScan(){

}

void Ip::m_startScan(){
    /*
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */
    int wordlistCount = m_targetListModel->rowCount();
    int threadsCount = m_scanArgs->config->threadsCount;
    if(threadsCount > wordlistCount)
        threadsCount = wordlistCount;

    status->ip->activeScanThreads = threadsCount;

    /* loop to create threads for enumeration... */
    for(int i = 0; i < threadsCount; i++)
    {
        ip::Scanner *scanner = new ip::Scanner(m_scanArgs);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        connect(scanner, &ip::Scanner::scanResult, this, &Ip::onScanResult);
        connect(scanner, &ip::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &ip::Scanner::infoLog, this, &Ip::onInfoLog);
        connect(scanner, &ip::Scanner::errorLog, this, &Ip::onErrorLog);
        connect(cThread, &QThread::finished, this, &Ip::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &ip::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Ip::stopScanThread, scanner, &ip::Scanner::onStopScan);
        cThread->start();
    }
    status->ip->isRunning = true;
}

void Ip::onScanThreadEnded(){
    status->ip->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->ip->activeScanThreads == 0)
    {
        if(status->ip->isPaused)
        {
            status->ip->isRunning = false;
            return;
        }
        else
        {
            /* set the progress bar to 100% just in case... */
            if(!status->ip->isStopped)
                ui->progressBar->setValue(ui->progressBar->maximum());

            status->ip->isPaused = false;
            status->ip->isStopped = false;
            status->ip->isRunning = false;

            /* enabling and diabling widgets */
            ui->buttonStart->setEnabled(true);
            ui->buttonStop->setDisabled(true);
        }
    }
}

void Ip::onScanResult(QString subdomain, QString ipAddress){
    /* save to ip model model... */
    m_resultModel->appendRow(QList<QStandardItem*>() <<new QStandardItem(ipAddress) <<new QStandardItem(subdomain));
    ui->labelResultsCount->setNum(m_resultModel->rowCount());

    /* save to project model... */
    project->addActiveSubdomain(QStringList()<<subdomain<<ipAddress<<subdomain);
}
