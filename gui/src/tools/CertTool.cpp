#include "CertTool.h"
#include "ui_CertTool.h"
//...
#include <QSslCertificate>
#include <QStandardItem>
#include <QSslKey>
#include "src/utils/Definitions.h"
#include "src/modules/scan/CertScanner.h"

#define ACTIVE 0
#define CRTSH 1

/*
 * just active and crtsh for now...
 *
 * articles on ssl-cert osint
 * https://osintcurio.us/2019/03/12/certificates-the-osint-gift-that-keeps-on-giving/
 */
CertTool::CertTool(QWidget *parent) : QDialog(parent), ui(new Ui::CertTool),
    m_certModel(new CertModel),
    m_proxyModel(new QSortFilterProxyModel),
    m_scanArgs(new ScanArgs)
{
    ui->setupUi(this);

    /* setting the placeholdertxt */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

    /* setting the models */
    m_proxyModel->setSourceModel(m_certModel->model);
    ui->treeResults->setModel(m_proxyModel);
}
CertTool::~CertTool(){
    delete ui;
    delete m_certModel;
    delete m_proxyModel;
    delete m_scanArgs;
}

void CertTool::onEnumerationComplete(){
    ui->buttonAnalyze->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void CertTool::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void CertTool::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void CertTool::onRawCert(QByteArray rawCert){
    QList<QSslCertificate> certList = QSslCertificate::fromData(rawCert, QSsl::Pem);

    foreach(const QSslCertificate &cert, certList)
    {
        /* ... */
        m_certModel->info_verison->setText(cert.version());
        m_certModel->info_serialNumber->setText(cert.serialNumber());
        m_certModel->info_signatureAlgorithm->setText(""); // none yet

        /* fingerprint */
        m_certModel->fingerprint_md5->setText(cert.digest(QCryptographicHash::Md5).toHex());
        m_certModel->fingerprint_sha1->setText(cert.digest(QCryptographicHash::Sha1).toHex());
        m_certModel->fingerprint_sha256->setText(cert.digest(QCryptographicHash::Sha256).toHex());

        /* validity */
        m_certModel->validity_notBefore->setText(cert.effectiveDate().toString());
        m_certModel->validity_notAfter->setText(cert.expiryDate().toString());

        /* issuer Info */
        if(cert.issuerInfo(QSslCertificate::CommonName).length() > 0)
            m_certModel->issuer_commonName->setText(cert.issuerInfo(QSslCertificate::CommonName)[0]);
        if(cert.issuerInfo(QSslCertificate::Organization).length() > 0)
            m_certModel->issuer_organizationName->setText(cert.issuerInfo(QSslCertificate::Organization)[0]);
        if(cert.issuerInfo(QSslCertificate::CountryName).length() > 0)
            m_certModel->issuer_countryName->setText(cert.issuerInfo(QSslCertificate::CountryName)[0]);

        /* subject info */
        if(cert.subjectInfo(QSslCertificate::CommonName).length() > 0)
            m_certModel->subject_commonName->setText(cert.subjectInfo(QSslCertificate::CommonName)[0]);
        if(cert.subjectInfo(QSslCertificate::CountryName).length() > 0)
            m_certModel->subject_countryName->setText(cert.subjectInfo(QSslCertificate::CountryName)[0]);
        if(cert.subjectInfo(QSslCertificate::LocalityName).length() > 0)
            m_certModel->subject_localityName->setText(cert.subjectInfo(QSslCertificate::LocalityName)[0]);
        if(cert.subjectInfo(QSslCertificate::Organization).length() > 0)
            m_certModel->subject_organizationName->setText(cert.subjectInfo(QSslCertificate::Organization)[0]);
        if(cert.subjectInfo(QSslCertificate::StateOrProvinceName).length() > 0)
            m_certModel->subject_stateOrProvinceName->setText(cert.subjectInfo(QSslCertificate::StateOrProvinceName)[0]);
        if(cert.subjectInfo(QSslCertificate::EmailAddress).length() > 0)
            m_certModel->subject_email->setText(cert.subjectInfo(QSslCertificate::EmailAddress)[0]);

        // key type...
        if(cert.publicKey().type() == QSsl::PrivateKey)
            m_certModel->key_type->setText("Private Key");
        if(cert.publicKey().type() == QSsl::PublicKey)
            m_certModel->key_type->setText("Public Key");

        // algorithm type...
        if(cert.publicKey().algorithm() == QSsl::Rsa)
            m_certModel->key_algorithm->setText("RSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dsa)
            m_certModel->key_algorithm->setText("DSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Ec)
            m_certModel->key_algorithm->setText("Elliptic Curve algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dh)
            m_certModel->key_algorithm->setText("Diffie-Hellman algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Opaque)
            m_certModel->key_algorithm->setText("BlackBox");

        /* raw cert */
        ui->plainTextEditRawCert->setPlainText(cert.toPem());
        /* raw key */
        ui->plainTextEditRawKey->setPlainText(cert.publicKey().toPem());

        /* ... */
        foreach(const QString &altName, cert.subjectAlternativeNames())
            m_certModel->subjectAltNames->appendRow(new QStandardItem(altName));
    }
}

void CertTool::on_buttonAnalyze_clicked(){
    m_scanArgs->target = ui->lineEditTarget->text();
    m_scanArgs->info = true;

    ui->buttonStop->setEnabled(true);
    ui->buttonAnalyze->setDisabled(true);

    QThread *cThread = new QThread;
    switch (ui->comboBoxEngine->currentIndex()) {
    case ACTIVE:
    {
        cert::Scanner *scanner = new cert::Scanner(ui->lineEditTarget->text());
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, &cert::Scanner::rawCert, this, &CertTool::onRawCert);
        connect(scanner, &cert::Scanner::errorLog, this, &CertTool::onErrorLog);
        connect(scanner, &cert::Scanner::infoLog, this, &CertTool::onInfoLog);
        connect(cThread, &QThread::finished, this, &CertTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, scanner, &cert::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case CRTSH:
    {
        Crtsh *crtsh = new Crtsh(m_scanArgs);
        crtsh->Enumerator(cThread);
        crtsh->moveToThread(cThread);
        //...
        connect(crtsh, &Crtsh::rawCert, this, &CertTool::onRawCert);
        connect(crtsh, &Crtsh::errorLog, this, &CertTool::onErrorLog);
        connect(crtsh, &Crtsh::infoLog, this, &CertTool::onInfoLog);
        connect(cThread, &QThread::finished, this, &CertTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    }
}

void CertTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void CertTool::on_comboBoxEngine_currentIndexChanged(int index){
    /* clear the target */
    ui->lineEditTarget->clear();

    /* set new placeholdertxt */
    switch(index){
    case ACTIVE:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
        break;
    case CRTSH:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_SSLCERT);
    }
}
