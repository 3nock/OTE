/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef CIDRMODEL_H
#define CIDRMODEL_H

#include <QStandardItem>
#include <QDate>
#include <QSet>


namespace s3s_struct {
struct CIDR {
    QString cidr;

    /* ... */
    QSet<QString> emailcontacts;
    QSet<QString> abusecontacts;
    QSet<QString> asns;

    /* info */
    QString info_prefix;
    QString info_ip;
    QString info_cidr;
    QString info_name;
    QString info_description;
    QString info_country;
    QString info_website;
    QString info_ownerAddress;

    /* rir */
    QString rir_name;
    QString rir_country;
    QString rir_prefix;
    QString rir_dateallocated;
};

}

namespace s3s_item {

class CIDR: public QStandardItem {
public:
    CIDR(): QStandardItem(),
        info(new QStandardItem("Info")),
        emailContacts(new QStandardItem("Email Contacts")),
        abuseContacts(new QStandardItem("Abuse Contacts")),
        rir(new QStandardItem("RIR Info")),
        asns(new QStandardItem("ASNs")),

        /* info */
        info_prefix(new QStandardItem),
        info_ip(new QStandardItem),
        info_cidr(new QStandardItem),
        info_name(new QStandardItem),
        info_country(new QStandardItem),
        info_description(new QStandardItem),
        info_website(new QStandardItem),
        info_ownerAddress(new QStandardItem),

        /* rir */
        rir_name(new QStandardItem),
        rir_prefix(new QStandardItem),
        rir_country(new QStandardItem),
        rir_dateAllocated(new QStandardItem)
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setForeground(Qt::white);
        emailContacts->setForeground(Qt::white);
        abuseContacts->setForeground(Qt::white);
        rir->setForeground(Qt::white);
        asns->setForeground(Qt::white);

        info->setIcon(QIcon(":/img/res/icons/folder2.png"));
        emailContacts->setIcon(QIcon(":/img/res/icons/folder2.png"));
        abuseContacts->setIcon(QIcon(":/img/res/icons/folder2.png"));
        rir->setIcon(QIcon(":/img/res/icons/folder2.png"));
        asns->setIcon(QIcon(":/img/res/icons/folder2.png"));

        info->appendRow({new QStandardItem("prefix"), info_prefix});
        info->appendRow({new QStandardItem("ip"), info_ip});
        info->appendRow({new QStandardItem("cidr"), info_cidr});
        info->appendRow({new QStandardItem("name"), info_name});
        info->appendRow({new QStandardItem("Country"), info_country});
        info->appendRow({new QStandardItem("description"), info_description});
        info->appendRow({new QStandardItem("website"), info_website});
        info->appendRow({new QStandardItem("owner's address"), info_ownerAddress});

        rir->appendRow({new QStandardItem("name"), rir_name});
        rir->appendRow({new QStandardItem("prefix"), rir_prefix});
        rir->appendRow({new QStandardItem("country"), rir_country});
        rir->appendRow({new QStandardItem("date allocated"), rir_dateAllocated});

        /* append to the cidr */
        this->appendRow(info);
        this->appendRow(emailContacts);
        this->appendRow(abuseContacts);
        this->appendRow(rir);
        this->appendRow(asns);
    }
    ~CIDR()
    {
    }

public:
    /* general */
    QStandardItem *info;
    QStandardItem *emailContacts;
    QStandardItem *abuseContacts;
    QStandardItem *rir;
    QStandardItem *asns;

    /* info */
    QStandardItem *info_prefix;
    QStandardItem *info_ip;
    QStandardItem *info_cidr;
    QStandardItem *info_name;
    QStandardItem *info_country;
    QStandardItem *info_description;
    QStandardItem *info_website;
    QStandardItem *info_ownerAddress;

    /* rir */
    QStandardItem *rir_name;
    QStandardItem *rir_prefix;
    QStandardItem *rir_country;
    QStandardItem *rir_dateAllocated;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::CIDR &cidr){
        this->setText(cidr.cidr);

        /* info */
        info_prefix->setText(cidr.info_prefix);
        info_ip->setText(cidr.info_ip);
        info_cidr->setText(cidr.info_cidr);
        info_name->setText(cidr.info_name);
        info_description->setText(cidr.info_description);
        info_country->setText(cidr.info_country);
        info_website->setText(cidr.info_website);
        info_ownerAddress->setText(cidr.info_ownerAddress);

        /* rir */
        rir_name->setText(cidr.rir_name);
        rir_country->setText(cidr.rir_country);
        rir_prefix->setText(cidr.rir_prefix);
        rir_dateAllocated->setText(cidr.rir_dateallocated);

        int count = 0;
        /* email contacts */
        foreach(const QString &value, cidr.emailcontacts){
            emailContacts->appendRow({new QStandardItem(QString::number(count)),
                                      new QStandardItem(value)});
            count++;
        }

        /* abuse contacts */
        count = 0;
        foreach(const QString &value, cidr.abusecontacts){
            abuseContacts->appendRow({new QStandardItem(QString::number(count)),
                                      new QStandardItem(value)});
            count++;
        }

        /* asns */
        count = 0;
        foreach(const QString &asn, cidr.asns){
            asns->appendRow({new QStandardItem(QString::number(count)),
                             new QStandardItem(asn)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};

}

s3s_struct::CIDR cidr_to_struct(s3s_item::CIDR*);

QJsonObject cidr_to_json(s3s_item::CIDR*);

void json_to_cidr(const QJsonObject&, s3s_item::CIDR*);

#endif // CIDRMODEL_H
