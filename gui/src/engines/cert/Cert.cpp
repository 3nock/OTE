#include "Cert.h"
#include "ui_Cert.h"
//...
#include <QSslKey>
#include "src/utils/Definitions.h"
#include "src/models/certmodel.h"
#include "src/modules/scan/CertScanner.h"


Cert::Cert(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Cert),
    m_args(new certificate::ScanArguments)
{
    ui->setupUi(this);

    ui->targets->init("Targets");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Cert");

    ui->progressBar->hide();
    ui->comboBoxOption->hide();
    ui->buttonStop->setDisabled(true);

    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

    ui->treeViewResults->setModel(result->cert->subdomainProxy);
    result->active->subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->active->subdomainIpProxy->setRecursiveFilteringEnabled(true);

    connectActions();
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}
Cert::~Cert(){
    delete ui;
    delete m_args;
}

void Cert::on_buttonStart_clicked(){
    ///
    /// check...
    ///
    if(!(ui->targets->listModel->rowCount() > 0)){
        QMessageBox::warning(this, "Error!", "Please Enter Targets for Enumeration!");
        return;
    }
    ///
    /// disabling and Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->setMaximum(ui->targets->listModel->rowCount());
    ///
    /// Resetting the scan arguments values...
    ///
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

    this->startScan(args);
}

void Cert::on_buttonStop_clicked(){

}

void Cert::onScanThreadEnded(){

}

void Cert::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Cert::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Cert::onScanResultSHA1(QString sha1){
    result->cert->sslCert->appendRow(new QStandardItem(sha1));
    ui->labelResultsCount->setNum(result->cert->sslCert->rowCount());
}

void Cert::onScanResultSHA256(QString sha256){
    result->cert->sslCert->appendRow(new QStandardItem(sha256));
    ui->labelResultsCount->setNum(result->cert->sslCert->rowCount());
}

void Cert::onScanResultCertInfo(QByteArray rawCert){
    QList<QSslCertificate> certList = QSslCertificate::fromData(rawCert, QSsl::Pem);
    CertModel *certModel = new CertModel;

    foreach(const QSslCertificate &cert, certList)
    {
        certModel->main->setText(cert.digest(QCryptographicHash::Sha1).toHex());

        /* ... */
        certModel->info_verison->setText(cert.version());
        certModel->info_serialNumber->setText(cert.serialNumber());
        certModel->info_signatureAlgorithm->setText(""); // none yet

        /* fingerprint */
        certModel->fingerprint_md5->setText(cert.digest(QCryptographicHash::Md5).toHex());
        certModel->fingerprint_sha1->setText(cert.digest(QCryptographicHash::Sha1).toHex());
        certModel->fingerprint_sha256->setText(cert.digest(QCryptographicHash::Sha256).toHex());

        /* validity */
        certModel->validity_notBefore->setText(cert.effectiveDate().toString());
        certModel->validity_notAfter->setText(cert.expiryDate().toString());

        /* issuer Info */
        if(cert.issuerInfo(QSslCertificate::CommonName).length() > 0)
            certModel->issuer_commonName->setText(cert.issuerInfo(QSslCertificate::CommonName)[0]);
        if(cert.issuerInfo(QSslCertificate::Organization).length() > 0)
            certModel->issuer_organizationName->setText(cert.issuerInfo(QSslCertificate::Organization)[0]);
        if(cert.issuerInfo(QSslCertificate::CountryName).length() > 0)
            certModel->issuer_countryName->setText(cert.issuerInfo(QSslCertificate::CountryName)[0]);

        /* subject info */
        if(cert.subjectInfo(QSslCertificate::CommonName).length() > 0)
            certModel->subject_commonName->setText(cert.subjectInfo(QSslCertificate::CommonName)[0]);
        if(cert.subjectInfo(QSslCertificate::CountryName).length() > 0)
            certModel->subject_countryName->setText(cert.subjectInfo(QSslCertificate::CountryName)[0]);
        if(cert.subjectInfo(QSslCertificate::LocalityName).length() > 0)
            certModel->subject_localityName->setText(cert.subjectInfo(QSslCertificate::LocalityName)[0]);
        if(cert.subjectInfo(QSslCertificate::Organization).length() > 0)
            certModel->subject_organizationName->setText(cert.subjectInfo(QSslCertificate::Organization)[0]);
        if(cert.subjectInfo(QSslCertificate::StateOrProvinceName).length() > 0)
            certModel->subject_stateOrProvinceName->setText(cert.subjectInfo(QSslCertificate::StateOrProvinceName)[0]);
        if(cert.subjectInfo(QSslCertificate::EmailAddress).length() > 0)
            certModel->subject_email->setText(cert.subjectInfo(QSslCertificate::EmailAddress)[0]);

        // key type...
        if(cert.publicKey().type() == QSsl::PrivateKey)
            certModel->key_type->setText("Private Key");
        if(cert.publicKey().type() == QSsl::PublicKey)
            certModel->key_type->setText("Public Key");

        // algorithm type...
        if(cert.publicKey().algorithm() == QSsl::Rsa)
            certModel->key_algorithm->setText("RSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dsa)
            certModel->key_algorithm->setText("DSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Ec)
            certModel->key_algorithm->setText("Elliptic Curve algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dh)
            certModel->key_algorithm->setText("Diffie-Hellman algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Opaque)
            certModel->key_algorithm->setText("BlackBox");

        /* ... */
        foreach(const QString &altName, cert.subjectAlternativeNames())
            certModel->subjectAltNames->appendRow(new QStandardItem(altName));
    }

    result->cert->certInfo->appendRow(certModel->main);
    ui->labelResultsCount->setNum(result->cert->certInfo->rowCount());
}

void Cert::onScanResultSubdomain(QString subdomain){
    result->cert->subdomain->appendRow(new QStandardItem(subdomain));
    ui->labelResultsCount->setNum(result->cert->subdomain->rowCount());
}

void Cert::onClearResults(){
    ui->labelResultsCount->clear();
    ///
    /// clear the progressbar...
    ///
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();

    switch (ui->comboBoxOutput->currentIndex()) {
    case OUTPUT_SUBDOMAIN:
        result->cert->subdomain->clear();
        ui->lineEditFilter->clear();
        break;
    case OUTPUT_SSLCERT:
        result->cert->sslCert->clear();
        ui->lineEditFilter->clear();
        break;
    case OUTPUT_CERTINFO:
        result->cert->certInfo->clear();
        ui->lineEditFilter->clear();
        break;
    }
}

void Cert::on_comboBoxOutput_currentIndexChanged(int index){
    switch (index) {
    case OUTPUT_SUBDOMAIN:
        ui->treeViewResults->setModel(result->cert->subdomainProxy);
        ui->comboBoxOption->hide();
        ui->labelResultsCount->setNum(result->cert->subdomainProxy->rowCount());
        break;
    case OUTPUT_SSLCERT:
        ui->treeViewResults->setModel(result->cert->sslCertProxy);
        ui->comboBoxOption->show();
        ui->labelResultsCount->setNum(result->cert->sslCertProxy->rowCount());
        break;
    case OUTPUT_CERTINFO:
        ui->treeViewResults->setModel(result->cert->certInfo);
        ui->comboBoxOption->hide();
        ui->labelResultsCount->setNum(result->cert->certInfoProxy->rowCount());
        break;
    }
}

void Cert::on_lineEditFilter_textChanged(const QString &filterKeyword){
    switch (ui->comboBoxOutput->currentIndex()) {
    case OUTPUT_SUBDOMAIN:
        result->cert->subdomainProxy->setFilterRegExp(filterKeyword);
        ui->treeViewResults->setModel(result->cert->subdomainProxy);
        ui->labelResultsCount->setNum(result->cert->subdomainProxy->rowCount());
        break;
    case OUTPUT_SSLCERT:
        result->cert->sslCertProxy->setFilterRegExp(filterKeyword);
        ui->treeViewResults->setModel(result->cert->sslCertProxy);
        ui->labelResultsCount->setNum(result->cert->sslCertProxy->rowCount());
        break;
    case OUTPUT_CERTINFO:
        result->cert->certInfoProxy->setFilterRegExp(filterKeyword);
        ui->treeViewResults->setModel(result->cert->certInfoProxy);
        ui->labelResultsCount->setNum(result->cert->certInfoProxy->rowCount());
        break;
    }
}

void Cert::startScan(certificate::ScanArguments *args){
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
        certificate::Scanner *scanner = new certificate::Scanner(*args);
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
        //...
        cThread->start();
    }
    status->active->isRunning = true;
}
