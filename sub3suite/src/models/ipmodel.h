/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef IPMODEL_H
#define IPMODEL_H

#include <QStandardItem>
#include <QSet>


namespace s3s_struct {

struct IP {
    /* info */
    QString info_ip;
    QString info_type;
    QString info_host;
    QString info_city;
    QString info_region;
    QString info_countryName;
    QString info_countryCode;
    QString info_zip;
    QString info_geoLocation;
    QString info_organization;
    QString info_timezone;
    QString info_currency;

    /* asnInfo */
    QString asnInfo_asn;
    QString asnInfo_name;
    QString asnInfo_domain;
    QString asnInfo_route;
    QString asnInfo_type;

    /* companyInfo */
    QString companyInfo_name;
    QString companyInfo_domain;
    QString companyInfo_type;

    /* privacyInfo */
    QString privacyInfo_vpn;
    QString privacyInfo_proxy;
    QString privacyInfo_tor;
    QString privacyInfo_hosting;
    QString privacyInfo_relay;
    QString privacyInfo_threatLevel;

    /* abuseInfo */
    QString abuseInfo_address;
    QString abuseInfo_country;
    QString abuseInfo_email;
    QString abuseInfo_name;
    QString abuseInfo_network;
    QString abuseInfo_phone;

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
        abuseInfo(new QStandardItem("Abuse Information")),
        domains(new QStandardItem("Domains")),

        /* info */
        info_ip(new QStandardItem),
        info_type(new QStandardItem),
        info_host(new QStandardItem),
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
        asnInfo_domain(new QStandardItem),
        asnInfo_route(new QStandardItem),
        asnInfo_type(new QStandardItem),

        /* companyInfo */
        companyInfo_name(new QStandardItem),
        companyInfo_domain(new QStandardItem),
        companyInfo_type(new QStandardItem),

        /* privacy info */
        privacyInfo_vpn(new QStandardItem),
        privacyInfo_proxy(new QStandardItem),
        privacyInfo_tor(new QStandardItem),
        privacyInfo_hosting(new QStandardItem),
        privacyInfo_relay(new QStandardItem),
        privacyInfo_threatLevel(new QStandardItem),

        /* abuse info */
        abuseInfo_address(new QStandardItem),
        abuseInfo_country(new QStandardItem),
        abuseInfo_email(new QStandardItem),
        abuseInfo_name(new QStandardItem),
        abuseInfo_network(new QStandardItem),
        abuseInfo_phone(new QStandardItem)
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setForeground(Qt::white);
        asnInfo->setForeground(Qt::white);
        companyInfo->setForeground(Qt::white);
        privacyInfo->setForeground(Qt::white);
        abuseInfo->setForeground(Qt::white);
        domains->setForeground(Qt::white);

        info->setIcon(QIcon(":/img/res/icons/folder2.png"));
        asnInfo->setIcon(QIcon(":/img/res/icons/folder2.png"));
        companyInfo->setIcon(QIcon(":/img/res/icons/folder2.png"));
        privacyInfo->setIcon(QIcon(":/img/res/icons/folder2.png"));
        abuseInfo->setIcon(QIcon(":/img/res/icons/folder2.png"));
        domains->setIcon(QIcon(":/img/res/icons/folder2.png"));

        info->appendRow({new QStandardItem("Ip"), info_ip});
        info->appendRow({new QStandardItem("Type"), info_type});
        info->appendRow({new QStandardItem("Hostname"), info_host});
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
        asnInfo->appendRow({new QStandardItem("Domain"), asnInfo_domain});
        asnInfo->appendRow({new QStandardItem("Route"), asnInfo_route});
        asnInfo->appendRow({new QStandardItem("Type"), asnInfo_type});

        companyInfo->appendRow({new QStandardItem("Name"), companyInfo_name});
        companyInfo->appendRow({new QStandardItem("Type"), companyInfo_type});
        companyInfo->appendRow({new QStandardItem("Domain"), companyInfo_domain});

        privacyInfo->appendRow({new QStandardItem("VPN"), privacyInfo_vpn});
        privacyInfo->appendRow({new QStandardItem("Proxy"), privacyInfo_proxy});
        privacyInfo->appendRow({new QStandardItem("Tor"), privacyInfo_tor});
        privacyInfo->appendRow({new QStandardItem("Hosting"), privacyInfo_hosting});
        privacyInfo->appendRow({new QStandardItem("Relay"), privacyInfo_relay});
        privacyInfo->appendRow({new QStandardItem("ThreatLevel"), privacyInfo_threatLevel});

        abuseInfo->appendRow({new QStandardItem("Address"), abuseInfo_address});
        abuseInfo->appendRow({new QStandardItem("Country"), abuseInfo_country});
        abuseInfo->appendRow({new QStandardItem("Email"), abuseInfo_email});
        abuseInfo->appendRow({new QStandardItem("Name"), abuseInfo_name});
        abuseInfo->appendRow({new QStandardItem("Network"), abuseInfo_network});
        abuseInfo->appendRow({new QStandardItem("Phone"), abuseInfo_phone});

        /* append to the IP */
        this->appendRow(info);
        this->appendRow(asnInfo);
        this->appendRow(companyInfo);
        this->appendRow(privacyInfo);
        this->appendRow(abuseInfo);
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
    QStandardItem *abuseInfo;
    QStandardItem *domains;

    /* info */
    QStandardItem *info_ip;
    QStandardItem *info_type;
    QStandardItem *info_host;
    QStandardItem *info_city;
    QStandardItem *info_region;
    QStandardItem *info_countryName;
    QStandardItem *info_countryCode;
    QStandardItem *info_zip;
    QStandardItem *info_geoLocation;
    QStandardItem *info_organization;
    QStandardItem *info_timezone;
    QStandardItem *info_currency;

    /* asnInfo */
    QStandardItem *asnInfo_asn;
    QStandardItem *asnInfo_name;
    QStandardItem *asnInfo_domain;
    QStandardItem *asnInfo_route;
    QStandardItem *asnInfo_type;

    /* companyInfo */
    QStandardItem *companyInfo_name;
    QStandardItem *companyInfo_domain;
    QStandardItem *companyInfo_type;

    /* privacyInfo */
    QStandardItem *privacyInfo_vpn;
    QStandardItem *privacyInfo_proxy;
    QStandardItem *privacyInfo_tor;
    QStandardItem *privacyInfo_hosting;
    QStandardItem *privacyInfo_relay;
    QStandardItem *privacyInfo_threatLevel;

    /* abuseInfo */
    QStandardItem *abuseInfo_address;
    QStandardItem *abuseInfo_country;
    QStandardItem *abuseInfo_email;
    QStandardItem *abuseInfo_name;
    QStandardItem *abuseInfo_network;
    QStandardItem *abuseInfo_phone;

    void setValues(const s3s_struct::IP &ip){
        /* ip info */
        info_ip->setText(ip.info_ip);
        info_type->setText(ip.info_type);
        info_host->setText(ip.info_host);
        info_city->setText(ip.info_city);
        info_region->setText(ip.info_region);
        info_countryName->setText(ip.info_countryName);
        info_countryCode->setText(ip.info_countryCode);
        info_zip->setText(ip.info_zip);
        info_geoLocation->setText(ip.info_geoLocation);
        info_organization->setText(ip.info_organization);
        info_timezone->setText(ip.info_timezone);
        info_currency->setText(ip.info_currency);

        /* asn info */
        asnInfo_asn->setText(ip.asnInfo_asn);
        asnInfo_name->setText(ip.asnInfo_name);
        asnInfo_domain->setText(ip.asnInfo_domain);
        asnInfo_route->setText(ip.asnInfo_route);
        asnInfo_type->setText(ip.asnInfo_type);

        /* company info */
        companyInfo_name->setText(ip.companyInfo_name);
        companyInfo_domain->setText(ip.companyInfo_domain);
        companyInfo_type->setText(ip.companyInfo_type);

        /* privacy info */
        privacyInfo_vpn->setText(ip.privacyInfo_vpn);
        privacyInfo_proxy->setText(ip.privacyInfo_proxy);
        privacyInfo_tor->setText(ip.privacyInfo_tor);
        privacyInfo_hosting->setText(ip.privacyInfo_hosting);
        privacyInfo_relay->setText(ip.privacyInfo_relay);
        privacyInfo_threatLevel->setText(ip.privacyInfo_threatLevel);

        /* abuse info */
        abuseInfo_address->setText(ip.abuseInfo_address);
        abuseInfo_country->setText(ip.abuseInfo_country);
        abuseInfo_email->setText(ip.abuseInfo_email);
        abuseInfo_name->setText(ip.abuseInfo_name);
        abuseInfo_network->setText(ip.abuseInfo_network);
        abuseInfo_phone->setText(ip.abuseInfo_phone);

        /* domains */
        int count = domains->rowCount();
        foreach(const QString &domain, ip.domains){
            domains->appendRow({new QStandardItem(QString::number(count)), new QStandardItem(domain)});
            count++;
        }
    }
};

}

s3s_struct::IP ip_to_struct(s3s_item::IP*);

QJsonObject ip_to_json(s3s_item::IP*);

void json_to_ip(const QJsonObject&, s3s_item::IP*);

#endif // IPMODEL_H
