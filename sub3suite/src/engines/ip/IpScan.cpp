#include "Ip.h"
#include "ui_Ip.h"

/*
 *
 *
 *
 */


void Ip::stopScan(){

}

void Ip::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just csubdomainIp the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->ip->isPaused)
    {
        //ui->buttonPause->setText("Pause");
        status->ip->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->ip->isPaused = true;
        emit stopScan();
    }
}

void Ip::ResumeScan(){

}

void Ip::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->targets->listModel->rowCount();
    int threadsCount = scanConfig->threadsCount;
    if(threadsCount > wordlistCount)
    {
        threadsCount = wordlistCount;
    }
    status->ip->activeThreads = threadsCount;
    ///
    /// loop to create threads for enumeration...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        ipEngine::Scanner *scanner = new ipEngine::Scanner(scanConfig, m_scanArguments);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, &ipEngine::Scanner::scanResult, this, &Ip::onScanResult);
        connect(scanner, &ipEngine::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &ipEngine::Scanner::infoLog, this, &Ip::onInfoLog);
        connect(scanner, &ipEngine::Scanner::errorLog, this, &Ip::onErrorLog);
        connect(cThread, &QThread::finished, this, &Ip::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &ipEngine::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Ip::stopScanThread, scanner, &ipEngine::Scanner::onStopScan);
        //...
        cThread->start();
    }
    status->ip->isRunning = true;
}

void Ip::onScanThreadEnded(){
    status->ip->activeThreads--;
    ///
    /// if subdomainIp Scan Threads have finished...
    ///
    if(status->ip->activeThreads == 0)
    {
        if(status->ip->isPaused)
        {
            //ui->buttonPause->setText("Resume");
            status->ip->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            return;
        }
        else
        {
            // set the progress bar to 100% just in case...
            if(!status->ip->isStopped){
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
            status->ip->isPaused = false;
            status->ip->isStopped = false;
            status->ip->isRunning = false;
            //...
            ui->buttonStart->setEnabled(true);
            //ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
        }
    }
}

void Ip::onScanResult(QString subdomain, QString ipAddress){
    ///
    /// save to ip model model...
    ///
    result->ip->subdomainIp->appendRow(QList<QStandardItem*>() <<new QStandardItem(ipAddress) <<new QStandardItem(subdomain));
    ui->labelResultsCount->setNum(result->ip->subdomainIp->rowCount());
    ///
    /// save to project model...
    ///
    project->addActiveSubdomain(QStringList()<<subdomain<<ipAddress<<subdomain);
}
