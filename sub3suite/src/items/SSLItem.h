/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef CERTMODEL_H
#define CERTMODEL_H

#include <QStandardItem>
#include <QSslCertificate>
#include <QSslKey>


namespace s3s_item {

class SSL: public QStandardItem {
public:
    SSL(): QStandardItem (),
        info(new QStandardItem("Info")),
        fingerprint(new QStandardItem("Fingerprint")),
        issuer(new QStandardItem("Issuer")),
        validity(new QStandardItem("Validity")),
        subject(new QStandardItem("Subject")),
        key(new QStandardItem("Key")),
        subjectAltNames(new QStandardItem("Subject Alternative Names")),
        /* info */
        info_verison(new QStandardItem),
        info_signatureAlgorithm(new QStandardItem),
        info_serialNumber(new QStandardItem),
        /* fingerprint */
        fingerprint_md5(new QStandardItem),
        fingerprint_sha1(new QStandardItem),
        fingerprint_sha256(new QStandardItem),
        /* issuer */
        issuer_commonName(new QStandardItem),
        issuer_organizationName(new QStandardItem),
        issuer_countryName(new QStandardItem),
        /* validity */
        validity_notBefore(new QStandardItem),
        validity_notAfter(new QStandardItem),
        /* subject */
        subject_commonName(new QStandardItem),
        subject_organizationName(new QStandardItem),
        subject_localityName(new QStandardItem),
        subject_stateOrProvinceName(new QStandardItem),
        subject_countryName(new QStandardItem),
        subject_email(new QStandardItem),
        /* key */
        key_type(new QStandardItem),
        key_algorithm(new QStandardItem)
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setForeground(Qt::white);
        fingerprint->setForeground(Qt::white);
        issuer->setForeground(Qt::white);
        validity->setForeground(Qt::white);
        subject->setForeground(Qt::white);
        key->setForeground(Qt::white);
        subjectAltNames->setForeground(Qt::white);

        info->setIcon(QIcon(":/img/res/icons/folder2.png"));
        fingerprint->setIcon(QIcon(":/img/res/icons/folder2.png"));
        issuer->setIcon(QIcon(":/img/res/icons/folder2.png"));
        validity->setIcon(QIcon(":/img/res/icons/folder2.png"));
        subject->setIcon(QIcon(":/img/res/icons/folder2.png"));
        key->setIcon(QIcon(":/img/res/icons/folder2.png"));
        subjectAltNames->setIcon(QIcon(":/img/res/icons/folder2.png"));

        info->appendRow({new QStandardItem("Version"), info_verison});
        info->appendRow({new QStandardItem("signature Algorithm"), info_signatureAlgorithm});
        info->appendRow({new QStandardItem("Serial Number"), info_serialNumber});

        fingerprint->appendRow({new QStandardItem("md5"), fingerprint_md5});
        fingerprint->appendRow({new QStandardItem("sha1"), fingerprint_sha1});
        fingerprint->appendRow({new QStandardItem("sha256"), fingerprint_sha256});

        issuer->appendRow({new QStandardItem("Common Name"), issuer_commonName});
        issuer->appendRow({new QStandardItem("Organization Name"), issuer_organizationName});
        issuer->appendRow({new QStandardItem("Country Name"), issuer_countryName});

        validity->appendRow({new QStandardItem("Not Before"), validity_notBefore});
        validity->appendRow({new QStandardItem("Not After"), validity_notAfter});

        subject->appendRow({new QStandardItem("Common Name"), subject_commonName});
        subject->appendRow({new QStandardItem("Organization Name"), subject_organizationName});
        subject->appendRow({new QStandardItem("Locality Name"), subject_localityName});
        subject->appendRow({new QStandardItem("State Or Province"), subject_stateOrProvinceName});
        subject->appendRow({new QStandardItem("Country Name"), subject_countryName});
        subject->appendRow({new QStandardItem("Email"), subject_email});

        key->appendRow({new QStandardItem("Type"), key_type});
        key->appendRow({new QStandardItem("Algorithm"), key_algorithm});

        /* appending to the SSL */
        this->appendRow(info);
        this->appendRow(fingerprint);
        this->appendRow(validity);
        this->appendRow(issuer);
        this->appendRow(subject);
        this->appendRow(key);
        this->appendRow(subjectAltNames);
    }
    ~SSL()
    {
    }

public:
    QStandardItem *info;
    QStandardItem *fingerprint;
    QStandardItem *issuer;
    QStandardItem *validity;
    QStandardItem *subject;
    QStandardItem *key;
    QStandardItem *subjectAltNames;

    /* info */
    QStandardItem *info_verison;
    QStandardItem *info_signatureAlgorithm;
    QStandardItem *info_serialNumber;

    /* fingerprint */
    QStandardItem *fingerprint_md5;
    QStandardItem *fingerprint_sha1;
    QStandardItem *fingerprint_sha256;

    /* issuer */
    QStandardItem *issuer_commonName;
    QStandardItem *issuer_organizationName;
    QStandardItem *issuer_countryName;

    /* validity */
    QStandardItem *validity_notBefore;
    QStandardItem *validity_notAfter;

    /* subject */
    QStandardItem *subject_commonName;
    QStandardItem *subject_organizationName;
    QStandardItem *subject_localityName;
    QStandardItem *subject_stateOrProvinceName;
    QStandardItem *subject_countryName;
    QStandardItem *subject_email;

    /* Key */
    QStandardItem *key_type;
    QStandardItem *key_algorithm;

    /* raw certificate */
    QByteArray raw;
    QByteArray raw_key;

    void setValues(const QString &target, const QSslCertificate &cert){
        this->setText(target);

        info_verison->setText(cert.version());
        info_serialNumber->setText(cert.serialNumber());
        info_signatureAlgorithm->setText(""); // none yet

        /* fingerprint */
        fingerprint_md5->setText(cert.digest(QCryptographicHash::Md5).toHex());
        fingerprint_sha1->setText(cert.digest(QCryptographicHash::Sha1).toHex());
        fingerprint_sha256->setText(cert.digest(QCryptographicHash::Sha256).toHex());

        /* validity */
        validity_notBefore->setText(cert.effectiveDate().toString());
        validity_notAfter->setText(cert.expiryDate().toString());

        /* issuer Info */
        if(cert.issuerInfo(QSslCertificate::CommonName).length() > 0)
            issuer_commonName->setText(cert.issuerInfo(QSslCertificate::CommonName)[0]);
        if(cert.issuerInfo(QSslCertificate::Organization).length() > 0)
            issuer_organizationName->setText(cert.issuerInfo(QSslCertificate::Organization)[0]);
        if(cert.issuerInfo(QSslCertificate::CountryName).length() > 0)
            issuer_countryName->setText(cert.issuerInfo(QSslCertificate::CountryName)[0]);

        /* subject info */
        if(cert.subjectInfo(QSslCertificate::CommonName).length() > 0)
            subject_commonName->setText(cert.subjectInfo(QSslCertificate::CommonName)[0]);
        if(cert.subjectInfo(QSslCertificate::CountryName).length() > 0)
            subject_countryName->setText(cert.subjectInfo(QSslCertificate::CountryName)[0]);
        if(cert.subjectInfo(QSslCertificate::LocalityName).length() > 0)
            subject_localityName->setText(cert.subjectInfo(QSslCertificate::LocalityName)[0]);
        if(cert.subjectInfo(QSslCertificate::Organization).length() > 0)
            subject_organizationName->setText(cert.subjectInfo(QSslCertificate::Organization)[0]);
        if(cert.subjectInfo(QSslCertificate::StateOrProvinceName).length() > 0)
            subject_stateOrProvinceName->setText(cert.subjectInfo(QSslCertificate::StateOrProvinceName)[0]);
        if(cert.subjectInfo(QSslCertificate::EmailAddress).length() > 0)
            subject_email->setText(cert.subjectInfo(QSslCertificate::EmailAddress)[0]);

        /* key type */
        if(cert.publicKey().type() == QSsl::PrivateKey)
            key_type->setText("Private Key");
        if(cert.publicKey().type() == QSsl::PublicKey)
            key_type->setText("Public Key");

        /* algorithm type */
        if(cert.publicKey().algorithm() == QSsl::Rsa)
            key_algorithm->setText("RSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dsa)
            key_algorithm->setText("DSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Ec)
            key_algorithm->setText("Elliptic Curve algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dh)
            key_algorithm->setText("Diffie-Hellman algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Opaque)
            key_algorithm->setText("BlackBox");

        /* alternative names */
        int count = 0;
        foreach(const QString &value, cert.subjectAlternativeNames()){
            subjectAltNames->appendRow({new QStandardItem(QString::number(count)),
                                        new QStandardItem(value)});
            count++;
        }

        raw = cert.toPem();
        raw_key = cert.publicKey().toPem();
    }
};
}

QJsonObject ssl_to_json(s3s_item::SSL*);

void json_to_ssl(const QJsonObject&, s3s_item::SSL*);

#endif // CERTMODEL_H
