/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/


#ifndef IPITEM_H
#define IPITEM_H

#include "src/utils/utils.h"

#include <QStandardItem>
#include <QDate>
#include <QSet>

#define JSON_ARRAY "arr"
#define JSON_OBJECT "obj"

namespace s3s_struct {
struct IP {
    QString ip;

    /* info */
    QString info_ip;
    QString info_type;
    QString info_city;
    QString info_region;
    QString info_countryName;
    QString info_countryCode;
    QString info_zip;
    QString info_geoLocation;
    QString info_organization;
    QString info_timezone;

    /* asnInfo */
    QString asnInfo_asn;
    QString asnInfo_name;
    QString asnInfo_route;

    /* companyInfo */
    QString companyInfo_name;
    QString companyInfo_domain;

    /* privacyInfo */
    bool privacyInfo_proxy;
    bool privacyInfo_anonymous;
    bool privacyInfo_tor;
    bool privacyInfo_attacker;
    bool privacyInfo_abuser;
    bool privacyInfo_threat;

    /* ... */
    QSet<QString> domains;
};
}


namespace s3s_item {

class IP: public QStandardItem{
public:
    IP(): QStandardItem(),

        /* general */
        info(new QStandardItem("Ip Information")),
        asnInfo(new QStandardItem("ASN information")),
        companyInfo(new QStandardItem("Company Information")),
        privacyInfo(new QStandardItem("Privacy Information")),
        domains(new QStandardItem("Domains")),

        /* info */
        info_ip(new QStandardItem),
        info_type(new QStandardItem),
        info_city(new QStandardItem),
        info_region(new QStandardItem),
        info_countryName(new QStandardItem),
        info_countryCode(new QStandardItem),
        info_zip(new QStandardItem),
        info_geoLocation(new QStandardItem),
        info_organization(new QStandardItem),
        info_timezone(new QStandardItem),

        /* asnInfo */
        asnInfo_asn(new QStandardItem),
        asnInfo_name(new QStandardItem),
        asnInfo_route(new QStandardItem),

        /* companyInfo */
        companyInfo_name(new QStandardItem),
        companyInfo_domain(new QStandardItem),

        /* privacy info */
        privacyInfo_proxy(new QStandardItem),
        privacyInfo_anonymous(new QStandardItem),
        privacyInfo_tor(new QStandardItem),
        privacyInfo_attacker(new QStandardItem),
        privacyInfo_abuser(new QStandardItem),
        privacyInfo_threat(new QStandardItem)
    {
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setWhatsThis(JSON_OBJECT);
        asnInfo->setWhatsThis(JSON_OBJECT);
        companyInfo->setWhatsThis(JSON_OBJECT);
        privacyInfo->setWhatsThis(JSON_OBJECT);
        domains->setWhatsThis(JSON_ARRAY);

        info->setIcon(QIcon(":/img/res/icons/folder2.png"));
        asnInfo->setIcon(QIcon(":/img/res/icons/folder2.png"));
        companyInfo->setIcon(QIcon(":/img/res/icons/folder2.png"));
        privacyInfo->setIcon(QIcon(":/img/res/icons/folder2.png"));
        domains->setIcon(QIcon(":/img/res/icons/folder2.png"));

        if(s3s_global::is_dark_theme){
            this->setForeground(Qt::white);
            info->setForeground(Qt::white);
            asnInfo->setForeground(Qt::white);
            companyInfo->setForeground(Qt::white);
            privacyInfo->setForeground(Qt::white);
            domains->setForeground(Qt::white);
        }

        info->appendRow({new QStandardItem("Ip"), info_ip});
        info->appendRow({new QStandardItem("Type"), info_type});
        info->appendRow({new QStandardItem("City"), info_city});
        info->appendRow({new QStandardItem("Region"), info_region});
        info->appendRow({new QStandardItem("Country Code"), info_countryCode});
        info->appendRow({new QStandardItem("Country Name"), info_countryName});
        info->appendRow({new QStandardItem("Zip Code"), info_zip});
        info->appendRow({new QStandardItem("Geo Location"), info_geoLocation});
        info->appendRow({new QStandardItem("Organization"), info_organization});
        info->appendRow({new QStandardItem("Timezone"), info_timezone});

        asnInfo->appendRow({new QStandardItem("ASN"), asnInfo_asn});
        asnInfo->appendRow({new QStandardItem("Name"), asnInfo_name});
        asnInfo->appendRow({new QStandardItem("Route"), asnInfo_route});

        companyInfo->appendRow({new QStandardItem("Name"), companyInfo_name});
        companyInfo->appendRow({new QStandardItem("Domain"), companyInfo_domain});

        privacyInfo->appendRow({new QStandardItem("is Proxy"), privacyInfo_proxy});
        privacyInfo->appendRow({new QStandardItem("is Anonymous"), privacyInfo_anonymous});
        privacyInfo->appendRow({new QStandardItem("is Attacker"), privacyInfo_attacker});
        privacyInfo->appendRow({new QStandardItem("is Abuser"), privacyInfo_abuser});
        privacyInfo->appendRow({new QStandardItem("is Threat"), privacyInfo_threat});
        privacyInfo->appendRow({new QStandardItem("is Tor"), privacyInfo_tor});

        /* append to the IP */
        this->appendRow(info);
        this->appendRow(asnInfo);
        this->appendRow(companyInfo);
        this->appendRow(privacyInfo);
        this->appendRow(domains);
    }
    ~IP()
    {
    }

public:
    QStandardItem *info;
    QStandardItem *asnInfo;
    QStandardItem *companyInfo;
    QStandardItem *privacyInfo;
    QStandardItem *domains;

    /* info */
    QStandardItem *info_ip;
    QStandardItem *info_type;
    QStandardItem *info_city;
    QStandardItem *info_region;
    QStandardItem *info_countryName;
    QStandardItem *info_countryCode;
    QStandardItem *info_zip;
    QStandardItem *info_geoLocation;
    QStandardItem *info_organization;
    QStandardItem *info_timezone;

    /* asnInfo */
    QStandardItem *asnInfo_asn;
    QStandardItem *asnInfo_name;
    QStandardItem *asnInfo_route;

    /* companyInfo */
    QStandardItem *companyInfo_name;
    QStandardItem *companyInfo_domain;

    /* privacyInfo */
    QStandardItem *privacyInfo_proxy;
    QStandardItem *privacyInfo_anonymous;
    QStandardItem *privacyInfo_tor;
    QStandardItem *privacyInfo_attacker;
    QStandardItem *privacyInfo_abuser;
    QStandardItem *privacyInfo_threat;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::IP &ip){
        this->setText(ip.ip);

        /* ip info */
        info_ip->setText(ip.info_ip);
        info_type->setText(ip.info_type);
        info_city->setText(ip.info_city);
        info_region->setText(ip.info_region);
        info_countryName->setText(ip.info_countryName);
        info_countryCode->setText(ip.info_countryCode);
        info_zip->setText(ip.info_zip);
        info_geoLocation->setText(ip.info_geoLocation);
        info_organization->setText(ip.info_organization);
        info_timezone->setText(ip.info_timezone);

        /* asn info */
        asnInfo_asn->setText(ip.asnInfo_asn);
        asnInfo_name->setText(ip.asnInfo_name);
        asnInfo_route->setText(ip.asnInfo_route);

        /* company info */
        companyInfo_name->setText(ip.companyInfo_name);
        companyInfo_domain->setText(ip.companyInfo_domain);

        /* privacy info */
        privacyInfo_proxy->setText(QVariant(ip.privacyInfo_proxy).toString());
        privacyInfo_anonymous->setText(QVariant(ip.privacyInfo_anonymous).toString());
        privacyInfo_tor->setText(QVariant(ip.privacyInfo_tor).toString());
        privacyInfo_attacker->setText(QVariant(ip.privacyInfo_attacker).toString());
        privacyInfo_abuser->setText(QVariant(ip.privacyInfo_abuser).toString());
        privacyInfo_threat->setText(QVariant(ip.privacyInfo_threat).toString());

        /* domains */
        int count = domains->rowCount();
        foreach(const QString &domain, ip.domains){
            domains->appendRow({new QStandardItem(QString::number(count)),
                                new QStandardItem(domain)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void addValues(const s3s_struct::IP &ip){
        /* ip info */
        if(info_ip->text().isEmpty())
            info_ip->setText(ip.info_ip);
        if(info_type->text().isEmpty())
            info_type->setText(ip.info_type);
        if(info_city->text().isEmpty())
            info_city->setText(ip.info_city);
        if(info_region->text().isEmpty())
            info_region->setText(ip.info_region);
        if(info_countryName->text().isEmpty())
            info_countryName->setText(ip.info_countryName);
        if(info_countryCode->text().isEmpty())
            info_countryCode->setText(ip.info_countryCode);
        if(info_zip->text().isEmpty())
            info_zip->setText(ip.info_zip);
        if(info_geoLocation->text().isEmpty())
            info_geoLocation->setText(ip.info_geoLocation);
        if(info_organization->text().isEmpty())
            info_organization->setText(ip.info_organization);
        if(info_timezone->text().isEmpty())
            info_timezone->setText(ip.info_timezone);

        /* asn info */
        if(asnInfo_asn->text().isEmpty())
            asnInfo_asn->setText(ip.asnInfo_asn);
        if(asnInfo_name->text().isEmpty())
            asnInfo_name->setText(ip.asnInfo_name);
        if(asnInfo_route->text().isEmpty())
            asnInfo_route->setText(ip.asnInfo_route);

        /* company info */
        if(companyInfo_name->text().isEmpty())
            companyInfo_name->setText(ip.companyInfo_name);
        if(companyInfo_domain->text().isEmpty())
            companyInfo_domain->setText(ip.companyInfo_domain);

        /* privacy info */
        if(privacyInfo_proxy->text().isEmpty())
            privacyInfo_proxy->setText(QVariant(ip.privacyInfo_proxy).toString());
        if(privacyInfo_anonymous->text().isEmpty())
            privacyInfo_anonymous->setText(QVariant(ip.privacyInfo_anonymous).toString());
        if(privacyInfo_tor->text().isEmpty())
            privacyInfo_tor->setText(QVariant(ip.privacyInfo_tor).toString());
        if(privacyInfo_attacker->text().isEmpty())
            privacyInfo_attacker->setText(QVariant(ip.privacyInfo_attacker).toString());
        if(privacyInfo_abuser->text().isEmpty())
            privacyInfo_abuser->setText(QVariant(ip.privacyInfo_abuser).toString());
        if(privacyInfo_threat->text().isEmpty())
            privacyInfo_threat->setText(QVariant(ip.privacyInfo_threat).toString());

        /* domains */
        int count = domains->rowCount();
        foreach(const QString &domain, ip.domains){
            domains->appendRow({new QStandardItem(QString::number(count)),
                                new QStandardItem(domain)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void addDomains(const s3s_struct::IP &ip){
        this->setText(ip.ip);
        int count = domains->rowCount();
        foreach(const QString &domain, ip.domains){
            domains->appendRow({new QStandardItem(QString::number(count)),
                                new QStandardItem(domain)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};

}

s3s_struct::IP ip_to_struct(s3s_item::IP*);

QJsonObject ip_to_json(s3s_item::IP*);

void json_to_ip(const QJsonObject&, s3s_item::IP*);

#endif // IPITEM_H
