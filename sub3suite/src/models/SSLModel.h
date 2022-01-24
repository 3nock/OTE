/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef CERTMODEL_H
#define CERTMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>


class CertModel{
public:
    CertModel():
        mainItem(new QStandardItem),
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
        mainItem->setForeground(Qt::white);
        info->setForeground(Qt::white);
        fingerprint->setForeground(Qt::white);
        issuer->setForeground(Qt::white);
        validity->setForeground(Qt::white);
        subject->setForeground(Qt::white);
        key->setForeground(Qt::white);
        subjectAltNames->setForeground(Qt::white);

        mainItem->setIcon(QIcon(":/img/res/icons/folder.png"));
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

        /* appending to the mainItem */
        mainItem->appendRow(info);
        mainItem->appendRow(fingerprint);
        mainItem->appendRow(validity);
        mainItem->appendRow(issuer);
        mainItem->appendRow(subject);
        mainItem->appendRow(key);
        mainItem->appendRow(subjectAltNames);
    }
    ~CertModel(){
    }

    QStandardItem *mainItem;

private:
    QStandardItem *info;
    QStandardItem *fingerprint;
    QStandardItem *issuer;
    QStandardItem *validity;
    QStandardItem *subject;
    QStandardItem *key;

public:
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
};

#endif // CERTMODEL_H
