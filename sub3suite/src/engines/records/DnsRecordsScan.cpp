#include "DnsRecords.h"
#include "ui_DnsRecords.h"


void DnsRecords::m_stopScan(){

}

void DnsRecords::m_pauseScan(){
    /*
     if the scan was already paused, then this current click is to
     Resume the scan, just csubdomainIp the startScan, with the same arguments and
     it will continue at where it ended...
    */
    if(status->dns->isPaused){
        status->dns->isPaused = false;
        this->m_startScan();
    }
    else{
        status->dns->isPaused = true;
        emit stopScanThread();
    }
}

void DnsRecords::m_resumeScan(){

}

void DnsRecords::m_startScan(){
    /*
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */
    int wordlistCount = m_targetListModel->rowCount();
    int srvWordlistCount = m_srvWordlitsModel->rowCount();
    int threadsCount = m_scanArgs->config->threadsCount;

    if((ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS) && (threadsCount > wordlistCount))
        threadsCount = wordlistCount;

    if((ui->comboBoxOption->currentIndex() == OPTION::SRV) && (threadsCount > srvWordlistCount))
        threadsCount = wordlistCount;

    status->dns->activeScanThreads = threadsCount;

    /* loop to create threads for scan... */
    for(int i = 0; i < threadsCount; i++)
    {
        records::Scanner *scanner = new records::Scanner(m_scanArgs);
        QThread *cThread = new QThread(this);

        if(ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS)
            scanner->startScan(cThread);
        if(ui->comboBoxOption->currentIndex() == OPTION::SRV)
            scanner->startScan_srv(cThread);

        scanner->moveToThread(cThread);

        if(m_scanArgs->RecordType_srv)
            connect(scanner, &records::Scanner::scanProgress, ui->progressBarSRV, &QProgressBar::setValue);
        else
            connect(scanner, &records::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);

        connect(scanner, &records::Scanner::infoLog, this, &DnsRecords::onInfoLog);
        connect(scanner, &records::Scanner::errorLog, this, &DnsRecords::onErrorLog);
        connect(scanner, &records::Scanner::scanResult, this, &DnsRecords::onScanResult);
        connect(cThread, &QThread::finished, this, &DnsRecords::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &QThread::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &DnsRecords::stopScanThread, scanner, &records::Scanner::onStopScan);
        cThread->start();
    }
    status->dns->isRunning = true;
}

void DnsRecords::onScanThreadEnded(){
    status->dns->activeScanThreads--;

    /* if subdomainIp Scan Threads have finished... */
    if(status->dns->activeScanThreads == 0)
    {
        if(status->dns->isPaused)
        {
            status->dns->isRunning = false;
            return;
        }
        else
        {
            /* set the progress bar to 100% just in case... */
            if(!status->dns->isStopped)
                ui->progressBar->setValue(ui->progressBar->maximum());

            status->dns->isPaused = false;
            status->dns->isStopped = false;
            status->dns->isRunning = false;

            /* ... */
            ui->buttonStart->setEnabled(true);
            ui->buttonStop->setDisabled(true);
        }
    }
}

void DnsRecords::onScanResult(records::Results results){
    if(m_scanArgs->RecordType_srv)
    {
        result->records->srv->appendRow(QList<QStandardItem*>() <<new QStandardItem(results.srvName) <<new QStandardItem(results.srvTarget) <<new QStandardItem(QString::number(results.srvPort)));
        project->addActiveSRV(QStringList() <<results.srvName <<results.srvTarget <<results.domain);
        ui->labelResultsCountSRV->setNum(result->records->srv->rowCount());
        return;
    }

    /* for other record types...*/
    QStandardItem *domainItem = new QStandardItem(results.domain);
    domainItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
    domainItem->setForeground(Qt::white);
    result->records->dns->invisibleRootItem()->appendRow(domainItem);
    ui->labelResultsCount->setNum(result->records->dns->invisibleRootItem()->rowCount());

    if(m_scanArgs->RecordType_a && !results.A.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("A");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(const QString &item: results.A)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveA(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_aaaa && !results.AAAA.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("AAAA");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.AAAA)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveAAAA(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_ns  && !results.NS.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("NS");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.NS)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveNS(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_mx && !results.MX.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("MX");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.MX)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveMX(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_txt && !results.TXT.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("TXT");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.TXT)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveTXT(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_cname  && !results.CNAME.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("CNAME");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.CNAME)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveCNAME(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }
}
