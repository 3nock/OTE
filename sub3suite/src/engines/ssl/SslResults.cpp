/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Ssl.h"
#include "ui_Ssl.h"

#include <QSslKey>
#include "src/models/SSLModel.h"


void Ssl::onScanResultSHA1(QString sha1){
    m_resultModelCertId->appendRow(new QStandardItem(sha1));
    ui->labelResultsCount->setNum(m_resultModelCertId->rowCount());
}

void Ssl::onScanResultSHA256(QString sha256){
    m_resultModelCertId->appendRow(new QStandardItem(sha256));
    ui->labelResultsCount->setNum(m_resultModelCertId->rowCount());
}

void Ssl::onScanResultCertInfo(QByteArray rawCert){

    foreach(const QSslCertificate &cert, QSslCertificate::fromData(rawCert, QSsl::Pem))
    {
        CertModel *certModel = new CertModel;
        certModel->initItem();
        certModel->mainItem->setText(cert.digest(QCryptographicHash::Sha1).toHex());

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

        /* key type */
        if(cert.publicKey().type() == QSsl::PrivateKey)
            certModel->key_type->setText("Private Key");
        if(cert.publicKey().type() == QSsl::PublicKey)
            certModel->key_type->setText("Public Key");

        /* algorithm type */
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
        int alternativeName = 0;
        foreach(const QString &value, cert.subjectAlternativeNames()){
            certModel->subjectAltNames->appendRow({new QStandardItem(QString::number(alternativeName)), new QStandardItem(value)});
            alternativeName++;
        }

        m_resultModelCertInfo->invisibleRootItem()->appendRow(certModel->mainItem);
    }
    ui->labelResultsCount->setNum(m_resultModelCertInfo->rowCount());
}

void Ssl::onScanResultSubdomain(QString subdomain){
    m_resultModelSubdomain->appendRow(new QStandardItem(subdomain));
    ui->labelResultsCount->setNum(m_resultModelSubdomain->rowCount());
}
