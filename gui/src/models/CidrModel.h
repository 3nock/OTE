#ifndef CIDRMODEL_H
#define CIDRMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QMap>


enum CIDR_MODEL{
    asn,
    name,
    description,
    country,
    website,
    owneraddress,
    emailcontacts,
    abusecontacts,
    peers,
    prefixes,
    rir_name,
    rir_country,
    rir_dateallocated
};

class CidrModel{
public:
    CidrModel(){}

public:
    QStandardItem *prefix;
    QStandardItem *ip;
    QStandardItem *cidr;
    QStandardItem *name;
    QStandardItem *description;
    QStandardItem *website;
    QStandardItem *ownerAddress;
    QStandardItem *emailContacts;
    QStandardItem *abuseContacts;
    QStandardItem *asns;
    QStandardItem *rir;

    QStandardItem *rir_name;
    QStandardItem *rir_prefix;
    QStandardItem *rir_country;
    QStandardItem *rir_dateAllocated;
};

#endif // CIDRMODEL_H
