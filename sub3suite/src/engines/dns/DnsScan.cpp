/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"


void Dns::m_stopScan(){

}

void Dns::m_pauseScan(){
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

void Dns::m_resumeScan(){

}

void Dns::m_startScan(){
    /*
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */
    int wordlistCount = m_targetListModel->rowCount();
    int srvWordlistCount = m_srvWordlitsModel->rowCount();
    int threadsCount = 0;

    if((!ui->checkBoxSRV->isChecked()) && (threadsCount > wordlistCount))
        threadsCount = wordlistCount;

    if((ui->checkBoxSRV->isChecked()) && (threadsCount > srvWordlistCount*wordlistCount))
        threadsCount = wordlistCount;

    status->activeScanThreads = threadsCount;

    /* loop to create threads for scan... */
    for(int i = 0; i < threadsCount; i++)
    {
        dns::Scanner *scanner = new dns::Scanner(m_scanArgs);
        QThread *cThread = new QThread(this);
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        connect(scanner, &dns::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &dns::Scanner::infoLog, this, &Dns::onInfoLog);
        connect(scanner, &dns::Scanner::errorLog, this, &Dns::onErrorLog);
        connect(scanner, &dns::Scanner::scanResult, this, &Dns::onScanResult);
        connect(cThread, &QThread::finished, this, &Dns::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &QThread::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Dns::stopScanThread, scanner, &dns::Scanner::onStopScan);
        cThread->start();
    }
    status->isRunning = true;
}

void Dns::onScanThreadEnded(){
    status->activeScanThreads--;

    /* if subdomainIp Scan Threads have finished... */
    if(status->activeScanThreads == 0)
    {
        if(status->isPaused)
        {
            status->isRunning = false;
            return;
        }
        else
        {
            /* set the progress bar to 100% just in case... */
            if(!status->isStopped)
                ui->progressBar->setValue(ui->progressBar->maximum());

            status->isPaused = false;
            status->isStopped = false;
            status->isRunning = false;

            /* ... */
            ui->buttonStart->setEnabled(true);
            ui->buttonStop->setDisabled(true);
        }
    }
}

void Dns::onScanResult(dns::ScanResult results){
    if(m_scanArgs->RecordType_srv)
    {
        /*
        result->records->srv->appendRow(QList<QStandardItem*>() <<new QStandardItem(results.srvName) <<new QStandardItem(results.srvTarget) <<new QStandardItem(QString::number(results.srvPort)));
        project->addActiveSRV(QStringList() <<results.srvName <<results.srvTarget <<results.domain);
        ui->labelResultsCountSRV->setNum(result->records->srv->rowCount());
        */
        return;
    }

    /* for other record types...*/
    QStandardItem *domainItem = new QStandardItem(results.target);
    domainItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
    domainItem->setForeground(Qt::white);
    m_resultModel->invisibleRootItem()->appendRow(domainItem);
    ui->labelResultsCount->setNum(m_resultModel->invisibleRootItem()->rowCount());

    if(m_scanArgs->RecordType_a && !results.A.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("A");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(const QString &item: results.A)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveA(QStringList()<<item<<results.target);
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
            project->addActiveAAAA(QStringList()<<item<<results.target);
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
            project->addActiveNS(QStringList()<<item<<results.target);
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
            project->addActiveMX(QStringList()<<item<<results.target);
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
            project->addActiveTXT(QStringList()<<item<<results.target);
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
            project->addActiveCNAME(QStringList()<<item<<results.target);
        }
        domainItem->appendRow(recordItem);
    }
}
