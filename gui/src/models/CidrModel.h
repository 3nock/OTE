#ifndef CIDRMODEL_H
#define CIDRMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QSet>

/* structure for sending CidrModel data along signals & slots */
struct CidrModelStruct{
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

/* the main Cidr results data Model */
class CidrModel{
public:
    CidrModel():
        model(new QStandardItemModel),

        /* general */
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
        info_description(new QStandardItem),
        info_website(new QStandardItem),
        info_ownerAddress(new QStandardItem),

        /* rir */
        rir_name(new QStandardItem),
        rir_prefix(new QStandardItem),
        rir_country(new QStandardItem),
        rir_dateAllocated(new QStandardItem)
    {
        info->setForeground(QColor(220,220,220));
        emailContacts->setForeground(QColor(220,220,220));
        abuseContacts->setForeground(QColor(220,220,220));
        rir->setForeground(QColor(220,220,220));
        asns->setForeground(QColor(220,220,220));

        QFont font("Segoe UI", 9, QFont::Bold);
        info->setFont(font);
        emailContacts->setFont(font);
        abuseContacts->setFont(font);
        rir->setFont(font);
        asns->setFont(font);

        info->appendRow({new QStandardItem("prefix"), info_prefix});
        info->appendRow({new QStandardItem("ip"), info_ip});
        info->appendRow({new QStandardItem("cidr"), info_cidr});
        info->appendRow({new QStandardItem("name"), info_name});
        info->appendRow({new QStandardItem("description"), info_description});
        info->appendRow({new QStandardItem("website"), info_website});
        info->appendRow({new QStandardItem("owner's address"), info_ownerAddress});

        rir->appendRow({new QStandardItem("name"), rir_name});
        rir->appendRow({new QStandardItem("prefix"), rir_prefix});
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
        model->appendRow(asns);
    }
    ~CidrModel()
    {
        /* not yet implemented */
    }

public:
    QStandardItemModel *model;

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
    QStandardItem *info_description;
    QStandardItem *info_website;
    QStandardItem *info_ownerAddress;

    /* rir */
    QStandardItem *rir_name;
    QStandardItem *rir_prefix;
    QStandardItem *rir_country;
    QStandardItem *rir_dateAllocated;
    QString aaa;
};

#endif // CIDRMODEL_H
