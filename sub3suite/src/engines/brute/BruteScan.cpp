#include "Brute.h"
#include "ui_Brute.h"


QString Brute::targetFilterSubdomain(QString target){
    target = target.trimmed();

    /* error modifications
    target = target.remove(" ");
    target = target.replace(',', '.');
    */

    return target;
}

QString Brute::targetFilterTLD(QString target){
    target = target.trimmed();

    /* error modifications
    target = target.remove(" ");
    target = target.replace(',', '.');
    */

    target = target.split(".",QString::SkipEmptyParts)[0];
    return target;
}

void Brute::m_stopScan(){

}

void Brute::m_pauseScan(){
    /*
     if the scan was already paused, then this current click is to
     Resume the scan, just csubdomainIp the startScan, with the same arguments and
     it will continue at where it ended...
    */
    if(status->isPaused){
        status->isPaused = false;
        this->m_startScan();
    }
    else{
        status->isPaused = true;
        emit stopScanThread();
    }
}

void Brute::m_resumeScan(){

}

void Brute::m_startScan(){
    /*
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */

    if(m_scanArgs->config->threads > m_scanArgs->wordlist.length())
        status->activeScanThreads = m_scanArgs->wordlist.length();
    else
        status->activeScanThreads = m_scanArgs->config->threads;

    /* loop to create threads for scan... */
    for(int i = 0; i < status->activeScanThreads; i++)
    {
        QThread *cThread = new QThread;
        brute::Scanner *scanner = new brute::Scanner(m_scanArgs);

        switch (ui->comboBoxOutput->currentIndex())
        {
        case brute::OUTPUT::SUBDOMAIN:
            scanner->startScanSubdomain(cThread);
            scanner->moveToThread(cThread);
            connect(scanner, &brute::Scanner::result, this, &Brute::onResultSubdomain);
            connect(scanner, &brute::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
            connect(scanner, &brute::Scanner::infoLog, this, &Brute::onInfoLog);
            connect(scanner, &brute::Scanner::errorLog, this, &Brute::onErrorLog);
            connect(cThread, &QThread::finished, this, &Brute::onScanThreadEnded);
            connect(cThread, &QThread::finished, scanner, &brute::Scanner::deleteLater);
            connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
            connect(this, &Brute::stopScanThread, scanner, &brute::Scanner::onStopScan);
            break;

        case brute::OUTPUT::TLD:
            scanner->startScanTLD(cThread);
            scanner->moveToThread(cThread);
            connect(scanner, &brute::Scanner::result, this, &Brute::onResultTLD);
            connect(scanner, &brute::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
            connect(scanner, &brute::Scanner::infoLog, this, &Brute::onInfoLog);
            connect(scanner, &brute::Scanner::errorLog, this, &Brute::onErrorLog);
            connect(cThread, &QThread::finished, this, &Brute::onScanThreadEnded);
            connect(cThread, &QThread::finished, scanner, &brute::Scanner::deleteLater);
            connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
            connect(this, &Brute::stopScanThread, scanner, &brute::Scanner::onStopScan);
        }

        cThread->start();
    }
    status->isRunning = true;
}

void Brute::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if subdomainIp Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        if(status->isPaused){
            status->isRunning = false;
            return;
        }
        else
        {
            /* set the progress bar to 100% just in case... */
            if(!status->isStopped)
                ui->progressBar->setValue(ui->progressBar->maximum());

            m_scanArgs->targets.clear();
            status->isPaused = false;
            status->isStopped = false;
            status->isRunning = false;

            /* enabling and disabling widgets */
            ui->buttonStart->setEnabled(true);
            ui->buttonStop->setDisabled(true);
        }
    }
}
