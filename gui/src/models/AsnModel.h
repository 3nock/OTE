#ifndef ASNMODEL_H
#define ASNMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QSet>

struct AsModelStruct{
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


class AsModel{
public:
    AsModel():
        model(new QStandardItemModel),

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
        info->setForeground(QColor(220,220,220));
        emailContacts->setForeground(QColor(220,220,220));
        abuseContacts->setForeground(QColor(220,220,220));
        rir->setForeground(QColor(220,220,220));
        peers->setForeground(QColor(220,220,220));
        prefixes->setForeground(QColor(220,220,220));

        QFont font("Segoe UI", 9, QFont::Bold);
        info->setFont(font);
        emailContacts->setFont(font);
        abuseContacts->setFont(font);
        rir->setFont(font);
        peers->setFont(font);
        prefixes->setFont(font);

        info->appendRow({new QStandardItem("asn"), info_asn});
        info->appendRow({new QStandardItem("name"), info_name});
        info->appendRow({new QStandardItem("description"), info_description});
        info->appendRow({new QStandardItem("country"), info_country});
        info->appendRow({new QStandardItem("website"), info_website});
        info->appendRow({new QStandardItem("owner's address"), info_ownerAddress});

        rir->appendRow({new QStandardItem("name"), rir_name});
        rir->appendRow({new QStandardItem("country"), rir_country});
        rir->appendRow({new QStandardItem("date allocated"), rir_dateAllocated});

        /* the model */
        model->setColumnCount(2);
        model->setHorizontalHeaderLabels({"  Property", "  Value"});

        /* append to the model */
        model->appendRow(info);
        model->appendRow(emailContacts);
        model->appendRow(abuseContacts);
        model->appendRow(rir);
        model->appendRow(peers);
        model->appendRow(prefixes);
    }
    ~AsModel(){}

public:
    QStandardItemModel *model;

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
};

#endif // ASNMODEL_H
