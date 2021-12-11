#include "Cert.h"
#include "ui_Cert.h"
//...
#include "src/modules/scan/CertScanner.h"

/*
 *
 *
 *
 */

void Cert::startScan(){

    certificate::ScanArguments *args = new certificate::ScanArguments;
    args->singleTarget = false;

    foreach(const QString &target, ui->targets->listModel->stringList())
        args->targetList.push(target);

    switch (ui->comboBoxOutput->currentIndex()) {
        case OUTPUT_SUBDOMAIN:
            args->subdomain = true;
            break;
        case OUTPUT_SSLCERT:
            switch (ui->comboBoxOption->currentIndex()) {
                case 0: // SHA1
                    args->sha1 = true;
                    break;
                case 1: // SHA256
                    args->sha256 = true;
                    break;
            }
            break;
        case OUTPUT_CERTINFO:
            args->raw = true;
            break;
    }
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
    status->cert->activeThreads = threadsCount;
    ///
    /// loop to create threads for enumeration...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        certificate::Scanner *scanner = new certificate::Scanner(args);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);

        ///
        /// results signals & slots...
        ///
        switch (ui->comboBoxOutput->currentIndex()) {
        case OUTPUT_SUBDOMAIN:
            connect(scanner, &certificate::Scanner::resultSubdomain, this, &Cert::onScanResultSubdomain);
            break;
        case OUTPUT_SSLCERT:
            switch (ui->comboBoxOption->currentIndex()) {
            case 0: // SHA1
                connect(scanner, &certificate::Scanner::resultSHA1, this, &Cert::onScanResultSHA1);
                break;
            case 1: // SHA256
                connect(scanner, &certificate::Scanner::resultSHA256, this, &Cert::onScanResultSHA256);
                break;
            }
            break;
        case OUTPUT_CERTINFO:
            connect(scanner, &certificate::Scanner::resultRaw, this, &Cert::onScanResultCertInfo);
            break;
        }
        connect(scanner, &certificate::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &certificate::Scanner::infoLog, this, &Cert::onInfoLog);
        connect(scanner, &certificate::Scanner::errorLog, this, &Cert::onErrorLog);
        connect(cThread, &QThread::finished, this, &Cert::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &certificate::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //connect(cThread, &QThread::finished, this, [=](){delete args;});
        connect(this, &Cert::stopScanThread, scanner, &certificate::Scanner::onStopScan);
        cThread->start();
    }
    status->active->isRunning = true;
}
