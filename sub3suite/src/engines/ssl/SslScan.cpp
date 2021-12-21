#include "Ssl.h"
#include "ui_Ssl.h"

#include "src/modules/scan/SSLScanner.h"


void Ssl::m_startScan(){

    certificate::ScanArgs *args = new certificate::ScanArgs;
    args->singleTarget = false;

    foreach(const QString &target, m_targetListModel->stringList())
        args->targetList.push(target);

    switch (ui->comboBoxOutput->currentIndex()) {
        case ssl::OUTPUT::SUBDOMAIN:
            args->subdomain = true;
            break;
        case ssl::OUTPUT::CERT_ID:
            switch (ui->comboBoxOption->currentIndex()) {
                case 0: // SHA1
                    args->sha1 = true;
                    break;
                case 1: // SHA256
                    args->sha256 = true;
                    break;
            }
            break;
        case ssl::OUTPUT::CERT_INFO:
            args->raw = true;
            break;
    }
    /*
     if the numner of threads is greater than the number of wordlists, set the
     number of threads to use to the number of wordlists available to avoid
     creating more threads than needed...
    */
    int wordlistCount = m_targetListModel->rowCount();
    int threadsCount = m_scanArgs->config->threadsCount;
    if(threadsCount > wordlistCount)
        threadsCount = wordlistCount;

    status->cert->activeScanThreads = threadsCount;

    /* loop to create threads for enumeration... */
    for(int i = 0; i < threadsCount; i++)
    {
        certificate::Scanner *scanner = new certificate::Scanner(args);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);

        /* results signals & slots... */
        switch (ui->comboBoxOutput->currentIndex()) {
        case ssl::OUTPUT::SUBDOMAIN:
            connect(scanner, &certificate::Scanner::resultSubdomain, this, &Ssl::onScanResultSubdomain);
            break;
        case ssl::OUTPUT::CERT_ID:
            switch (ui->comboBoxOption->currentIndex()) {
            case 0: // SHA1
                connect(scanner, &certificate::Scanner::resultSHA1, this, &Ssl::onScanResultSHA1);
                break;
            case 1: // SHA256
                connect(scanner, &certificate::Scanner::resultSHA256, this, &Ssl::onScanResultSHA256);
                break;
            }
            break;
        case ssl::OUTPUT::CERT_INFO:
            connect(scanner, &certificate::Scanner::resultRaw, this, &Ssl::onScanResultCertInfo);
            break;
        }
        connect(scanner, &certificate::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &certificate::Scanner::infoLog, this, &Ssl::onInfoLog);
        connect(scanner, &certificate::Scanner::errorLog, this, &Ssl::onErrorLog);
        connect(cThread, &QThread::finished, this, &Ssl::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &certificate::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Ssl::stopScanThread, scanner, &certificate::Scanner::onStopScan);
        cThread->start();
    }
    status->active->isRunning = true;
}
