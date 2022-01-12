/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef IPMODEL_H
#define IPMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QSet>


struct IpModelStruct{
    QSet<QString> domains;
};

class IpModel{
public:
    IpModel():
        model(new QStandardItemModel),

        /* main */
        info(new QStandardItem("Ip Basic Information")),
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
        info->setForeground(QColor(220,220,220));
        asnInfo->setForeground(QColor(220,220,220));
        companyInfo->setForeground(QColor(220,220,220));
        privacyInfo->setForeground(QColor(220,220,220));
        abuseInfo->setForeground(QColor(220,220,220));
        domains->setForeground(QColor(220,220,220));

        QFont font("Segoe UI", 9, QFont::Bold);
        //...
        info->setFont(font);
        asnInfo->setFont(font);
        companyInfo->setFont(font);
        privacyInfo->setFont(font);
        abuseInfo->setFont(font);
        domains->setFont(font);

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

        /* the model */
        model->setColumnCount(2);
        model->setHorizontalHeaderLabels({"  Property", "  Value"});

        /* append to the model */
        model->appendRow(info);
        model->appendRow(asnInfo);
        model->appendRow(companyInfo);
        model->appendRow(privacyInfo);
        model->appendRow(abuseInfo);
        model->appendRow(domains);
    }
    ~IpModel(){
        delete model;
    }

public:
    QStandardItemModel *model;

    /* ... */
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
};

#endif // IPMODEL_H
