#ifndef ASNMODEL_H
#define ASNMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class AsModel{
public:
    AsModel(){}

public:
    QStandardItemModel *model;
    ///
    /// ...
    ///
    QStandardItem *asn;
    QStandardItem *name;
    QStandardItem *description;
    QStandardItem *countryCode;
    QStandardItem *ownerAddress;
    QStandardItem *emailContacts;
    QStandardItem *peers;
    QStandardItem *prefixes;
};

#endif // ASNMODEL_H
