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
    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    m_scanArgs->targets.clear();
    m_scanArgs->wordlist = m_wordlistModel->stringList();

    switch (ui->comboBoxOutput->currentIndex())
    {
    case brute::OUTPUT::SUBDOMAIN:
        if(ui->checkBoxMultipleTargets->isChecked()){
            foreach(const QString &target, m_targetListModel->stringList())
                m_scanArgs->targets.enqueue(this->targetFilterSubdomain(target));
        }
        else
            m_scanArgs->targets.enqueue(this->targetFilterSubdomain(ui->lineEditTarget->text()));
        m_scanArgs->output = brute::OUTPUT::SUBDOMAIN;
        break;

    case brute::OUTPUT::TLD:
        if(ui->checkBoxMultipleTargets->isChecked()){
            foreach(const QString &target, m_targetListModel->stringList())
                m_scanArgs->targets.enqueue(this->targetFilterTLD(target));
        }
        else
            m_scanArgs->targets.enqueue(this->targetFilterTLD(ui->lineEditTarget->text()));
        m_scanArgs->output = brute::OUTPUT::TLD;
    }

    /* set progressbar maximum value then set the first target & wordlist */
    ui->progressBar->setMaximum(m_scanArgs->wordlist.length()*m_scanArgs->targets.length());
    m_scanArgs->currentTarget = m_scanArgs->targets.dequeue();
    m_scanArgs->currentWordlist = 0;

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
        /*  TODO:
         *      set each scanner object & thread with specific nameserver
         */
        brute::Scanner *scanner = new brute::Scanner(m_scanArgs);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);

        switch (ui->comboBoxOutput->currentIndex()){
        case brute::OUTPUT::SUBDOMAIN:
            connect(scanner, &brute::Scanner::scanResult, this, &Brute::onResultSubdomain);
            break;
        case brute::OUTPUT::TLD:
            connect(scanner, &brute::Scanner::scanResult, this, &Brute::onResultTLD);
        }
        connect(scanner, &brute::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &brute::Scanner::scanLog, this, &Brute::onScanLog);
        connect(cThread, &QThread::finished, this, &Brute::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &brute::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Brute::stopScanThread, scanner, &brute::Scanner::onStopScan);

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

            this->m_log("------------------ End --------------\n");
        }
    }
}
