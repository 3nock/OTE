/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ASNMODEL_H
#define ASNMODEL_H

#include <QStandardItem>
#include <QJsonObject>
#include <QDate>
#include <QSet>


namespace s3s_struct {
struct ASN {
    QString asn;

    /* ... */
    QSet<QString> emailcontacts;
    QSet<QString> abusecontacts;
    QSet<QString> peers;
    QSet<QString> prefixes;

    /* info */
    QString info_asn;
    QString info_name;
    QString info_description;
    QString info_country;
    QString info_website;
    QString info_ownerAddress;

    /* rir */
    QString rir_name;
    QString rir_country;
    QString rir_dateallocated;
};
}


namespace s3s_item {

class ASN: public QStandardItem
{
public:
    ASN(): QStandardItem(),
        /* general */
        info(new QStandardItem("Info")),
        emailContacts(new QStandardItem("Email Contacts")),
        abuseContacts(new QStandardItem("Abuse Contacts")),
        rir(new QStandardItem("RIR allocation")),
        peers(new QStandardItem("Peers")),
        prefixes(new QStandardItem("Prefixes")),

        /* info */
        info_asn(new QStandardItem),
        info_name(new QStandardItem),
        info_description(new QStandardItem),
        info_country(new QStandardItem),
        info_website(new QStandardItem),
        info_ownerAddress(new QStandardItem),

        /* rir */
        rir_name(new QStandardItem),
        rir_country(new QStandardItem),
        rir_dateAllocated(new QStandardItem)
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setForeground(Qt::white);
        emailContacts->setForeground(Qt::white);
        abuseContacts->setForeground(Qt::white);
        rir->setForeground(Qt::white);
        peers->setForeground(Qt::white);
        prefixes->setForeground(Qt::white);

        info->setIcon(QIcon(":/img/res/icons/folder2.png"));
        emailContacts->setIcon(QIcon(":/img/res/icons/folder2.png"));
        abuseContacts->setIcon(QIcon(":/img/res/icons/folder2.png"));
        rir->setIcon(QIcon(":/img/res/icons/folder2.png"));
        peers->setIcon(QIcon(":/img/res/icons/folder2.png"));
        prefixes->setIcon(QIcon(":/img/res/icons/folder2.png"));

        info->appendRow({new QStandardItem("asn"), info_asn});
        info->appendRow({new QStandardItem("name"), info_name});
        info->appendRow({new QStandardItem("description"), info_description});
        info->appendRow({new QStandardItem("country"), info_country});
        info->appendRow({new QStandardItem("website"), info_website});
        info->appendRow({new QStandardItem("owner's address"), info_ownerAddress});

        rir->appendRow({new QStandardItem("name"), rir_name});
        rir->appendRow({new QStandardItem("country"), rir_country});
        rir->appendRow({new QStandardItem("date allocated"), rir_dateAllocated});

        /* append to the asn */
        this->appendRow(info);
        this->appendRow(emailContacts);
        this->appendRow(abuseContacts);
        this->appendRow(rir);
        this->appendRow(peers);
        this->appendRow(prefixes);
    }
    ~ASN()
    {
    }

public:
    /* general */
    QStandardItem *info;
    QStandardItem *emailContacts;
    QStandardItem *abuseContacts;
    QStandardItem *rir;
    QStandardItem *peers;
    QStandardItem *prefixes;

    /* info */
    QStandardItem *info_asn;
    QStandardItem *info_name;
    QStandardItem *info_description;
    QStandardItem *info_country;
    QStandardItem *info_website;
    QStandardItem *info_ownerAddress;

    /* rir */
    QStandardItem *rir_name;
    QStandardItem *rir_country;
    QStandardItem *rir_dateAllocated;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::ASN &asn){
        this->setText(asn.asn);

        /* info */
        info_asn->setText(asn.info_asn);
        info_name->setText(asn.info_name);
        info_description->setText(asn.info_description);
        info_country->setText(asn.info_country);
        info_website->setText(asn.info_website);
        info_ownerAddress->setText(asn.info_ownerAddress);

        /* rir */
        rir_name->setText(asn.rir_name);
        rir_country->setText(asn.rir_country);
        rir_dateAllocated->setText(asn.rir_dateallocated);

        int count = 0;
        /* email contacts */
        foreach(const QString &value, asn.emailcontacts){
            emailContacts->appendRow({new QStandardItem(QString::number(count)),
                                      new QStandardItem(value)});
            count++;
        }

        /* abuse contacts */
        count = 0;
        foreach(const QString &value, asn.abusecontacts){
            abuseContacts->appendRow({new QStandardItem(QString::number(count)),
                                      new QStandardItem(value)});
            count++;
        }

        /* peers */
        count = 0;
        foreach(const QString &peer, asn.peers){
            peers->appendRow({new QStandardItem(QString::number(count)),
                              new QStandardItem(peer)});
            count++;
        }

        /* prefixes */
        count = 0;
        foreach(const QString &prefix, asn.prefixes){
            prefixes->appendRow({new QStandardItem(QString::number(count)),
                                 new QStandardItem(prefix)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void setPeers(const QSet<QString> &peersList){
        int count = 0;
        foreach(const QString &peer, peersList){
            peers->appendRow({new QStandardItem(QString::number(count)),
                              new QStandardItem(peer)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void setPrefixes(const QSet<QString> &prefixList){
        int count = 0;
        foreach(const QString &prefix, prefixList){
            prefixes->appendRow({new QStandardItem(QString::number(count)),
                                 new QStandardItem(prefix)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};

}

s3s_struct::ASN asn_to_struct(s3s_item::ASN*);

QJsonObject asn_to_json(s3s_item::ASN*);

void json_to_asn(const QJsonObject&, s3s_item::ASN*);

#endif // ASNMODEL_H
