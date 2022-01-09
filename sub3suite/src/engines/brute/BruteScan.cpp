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

void Brute::m_startScan(){
    /* ressetting and setting new values */
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->clearMask();

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

    /*
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */
    if(m_scanArgs->config->threads > m_scanArgs->wordlist.length())
        status->activeScanThreads = m_scanArgs->wordlist.length();
    else
        status->activeScanThreads = m_scanArgs->config->threads;

    /* renewing scan statistics */
    m_scanStats->failed = 0;
    m_scanStats->resolved = 0;
    m_scanStats->threads = status->activeScanThreads;
    m_scanStats->wordlist = m_scanArgs->wordlist.size();
    m_scanStats->targets = m_scanArgs->targets.length();
    m_scanStats->nameservers = m_scanArgs->config->nameservers.length();

    /* set progressbar maximum value then set the first target & wordlist */
    ui->progressBar->setMaximum(m_scanArgs->wordlist.length()*m_scanArgs->targets.length());
    m_scanArgs->currentTarget = m_scanArgs->targets.dequeue();
    m_scanArgs->currentWordlist = 0;
    m_scanArgs->progress = 0;

    /* start timer */
    m_timer.start();

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
        connect(this, &Brute::pauseScanThread, scanner, &brute::Scanner::onPauseScan);
        connect(this, &Brute::resumeScanThread, scanner, &brute::Scanner::onResumeScan, Qt::DirectConnection);

        cThread->start();
    }
    status->isRunning = true;
}

void Brute::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if all Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        /* display the scan summary */
        m_scanSummary();

        if(status->isStopped)
            m_log("---------------- Stopped ------------\n");
        else
            m_log("------------------ End --------------\n");

        /* set the progress bar to 100% just in case... */
        if(!status->isStopped)
            ui->progressBar->setValue(ui->progressBar->maximum());

        m_scanArgs->targets.clear();
        m_scanArgs->wordlist.clear();
        m_scanArgs->currentTarget.clear();
        m_scanArgs->currentWordlist = 0;

        status->isNotActive = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isRunning = false;

        ui->buttonStart->setText("Start");
        ui->buttonStop->setDisabled(true);
    }
}

void Brute::m_scanSummary(){
    /* elapsed time */
    QTime time = QTime::fromMSecsSinceStartOfDay(m_timer.elapsed());

    /* write to log file */
    ui->plainTextEditLogs->appendHtml("<font color=\"white\">  [Scan Summary]</font>");
    ui->plainTextEditLogs->appendHtml("[ Resolved ]    : <font color=\"green\">"+QString::number(m_scanStats->resolved)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Failed ]      : <font color=\"red\">"+QString::number(m_scanStats->failed)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Threads ]     : <font color=\"green\">"+QString::number(m_scanStats->threads)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Targets ]     : <font color=\"green\">"+QString::number(m_scanStats->targets)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Wordlist ]    : <font color=\"green\">"+QString::number(m_scanStats->wordlist)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Nameservers ] : <font color=\"green\">"+QString::number(m_scanStats->nameservers)+"</font>");
    ui->plainTextEditLogs->appendHtml("[ Time (hh:mm:ss:zzz) ]        : <font color=\"green\">"+time.toString("hh:mm:ss:zzz")+"</font>");
}
