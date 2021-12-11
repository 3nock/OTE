#include "Brute.h"
#include "ui_Brute.h"

/*
 *
 *
 *
 */

void Brute::stopScan(){

}

void Brute::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just csubdomainIp the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->brute->isPaused){
        //ui->buttonPause->setText("Pause");
        status->brute->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->brute->isPaused = true;
        emit stopScan();
    }
}

void Brute::resumeScan(){

}

void Brute::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->wordlist->listModel->rowCount();
    int threadsCount = scanConfig->threadsCount;
    if(threadsCount > wordlistCount)
    {
        threadsCount = wordlistCount;
    }
    status->brute->activeThreads = threadsCount;
    ///
    /// loop to create threads for scan...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        QThread *cThread = new QThread;
        brute::Scanner *scanner = new brute::Scanner(scanConfig, m_scanArguments);
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, &brute::Scanner::scanResult, this, &Brute::onScanResult);
        connect(scanner, &brute::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &brute::Scanner::infoLog, this, &Brute::onInfoLog);
        connect(scanner, &brute::Scanner::errorLog, this, &Brute::onErrorLog);
        connect(cThread, &QThread::finished, this, &Brute::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &brute::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Brute::stopScanThread, scanner, &brute::Scanner::onStopScan);
        //...
        cThread->start();
    }
    status->brute->isRunning = true;
}

void Brute::onScanResult(QString subdomain, QString ipAddress, QString target){
    if(m_subdomainsSet.contains(subdomain)){
        return;
    }
    m_subdomainsSet.insert(subdomain);
    ///
    /// save to brute model...
    ///
    result->brute->subdomainIp->appendRow(QList<QStandardItem*>() <<new QStandardItem(subdomain) <<new QStandardItem(ipAddress));
    ui->labelResultsCount->setNum(result->brute->subdomainIp->rowCount());
    ///
    /// save to Project model...
    ///
    if(m_scanArguments->tldBrute){
        project->addActiveTLD(QStringList()<<subdomain<<ipAddress<<target);
    }
    if(m_scanArguments->subBrute){
        project->addActiveSubdomain(QStringList()<<subdomain<<ipAddress<<target);
    }
}

void Brute::onScanThreadEnded(){
    status->brute->activeThreads--;
    ///
    /// if subdomainIp Scan Threads have finished...
    ///
    if(status->brute->activeThreads == 0)
    {
        if(status->brute->isPaused)
        {
            //ui->buttonPause->setText("Resume");
            status->brute->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            return;
        }
        else
        {
            // set the progress bar to 100% just in case...
            if(!status->brute->isStopped){
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
            m_scanArguments->targetList.clear();
            status->brute->isPaused = false;
            status->brute->isStopped = false;
            status->brute->isRunning = false;
            //...
            ui->buttonStart->setEnabled(true);
            //ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
        }
    }
}
