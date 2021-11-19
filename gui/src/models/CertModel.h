#ifndef CERTMODEL_H
#define CERTMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class CertModel{
public:
    CertModel():
        model(new QStandardItemModel),
        /* ... */
        info(new QStandardItem("Info")),
        fingerprint(new QStandardItem("Fingerprint")),
        issuer(new QStandardItem("Issuer")),
        validity(new QStandardItem("Validity")),
        subject(new QStandardItem("Subject")),
        publicKey(new QStandardItem("Public key")),
        subjectAltNames(new QStandardItem("Subject Alternative Names")),
        /* info */
        info_verison(new QStandardItem),
        info_signatureAlgorithm(new QStandardItem),
        info_serialNumber(new QStandardItem),
        /* fingerprint */
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
        /* publicKey */
        publicKey_algorithm(new QStandardItem),
        publicKey_exponent(new QStandardItem),
        publicKey_modulus(new QStandardItem),
        publicKey_value(new QStandardItem)
    {
        info->setForeground(QColor(220,220,220));
        fingerprint->setForeground(QColor(220,220,220));
        issuer->setForeground(QColor(220,220,220));
        validity->setForeground(QColor(220,220,220));
        subject->setForeground(QColor(220,220,220));
        publicKey->setForeground(QColor(220,220,220));
        subjectAltNames->setForeground(QColor(220,220,220));

        QFont font("Segoe UI", 9, QFont::Bold);
        //...
        info->setFont(font);
        fingerprint->setFont(font);
        issuer->setFont(font);
        validity->setFont(font);
        subject->setFont(font);
        publicKey->setFont(font);
        subjectAltNames->setFont(font);

        info->appendRow({new QStandardItem("Version"), info_verison});
        info->appendRow({new QStandardItem("signature Algorithm"), info_signatureAlgorithm});
        info->appendRow({new QStandardItem("Serial Number"), info_serialNumber});

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

        publicKey->appendRow({new QStandardItem("Algorithm"), publicKey_algorithm});
        publicKey->appendRow({new QStandardItem("Exponent"), publicKey_exponent});
        publicKey->appendRow({new QStandardItem("Modulus"), publicKey_modulus});
        publicKey->appendRow({new QStandardItem("Value"), publicKey_value});

        /* the model */
        model->setColumnCount(2);
        model->setHorizontalHeaderLabels({"  Property", "  Value"});

        /* appending to the model */
        model->appendRow(info);
        model->appendRow(fingerprint);
        model->appendRow(issuer);
        model->appendRow(validity);
        model->appendRow(subject);
        model->appendRow(publicKey);
        model->appendRow(subjectAltNames);
    }
    ~CertModel(){
        delete model;
    }

public:
    QStandardItemModel *model;

    /* ... */
    QStandardItem *info;
    QStandardItem *fingerprint;
    QStandardItem *issuer;
    QStandardItem *validity;
    QStandardItem *subject;
    QStandardItem *publicKey;
    QStandardItem *subjectAltNames;

    /* info */
    QStandardItem *info_verison;
    QStandardItem *info_signatureAlgorithm;
    QStandardItem *info_serialNumber;

    /* fingerprint */
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

    /* publicKey */
    QStandardItem *publicKey_algorithm;
    QStandardItem *publicKey_exponent;
    QStandardItem *publicKey_modulus;
    QStandardItem *publicKey_value;
};

#endif // CERTMODEL_H
