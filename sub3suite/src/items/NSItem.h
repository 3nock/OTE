/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef NSITEM_H
#define NSITEM_H

#include <QStandardItem>
#include <QDate>
#include <QSet>

#define JSON_ARRAY "arr"
#define JSON_OBJECT "obj"

namespace s3s_struct {
struct NS {
    QString ns;
    QSet<QString> ip;
    QSet<QString> domains;
};
}

namespace s3s_item {

class NS: public QStandardItem {
public:
    NS(): QStandardItem (),
        ip(new QStandardItem("IPs")),
        domains(new QStandardItem("Domains"))
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        ip->setForeground(Qt::white);
        domains->setForeground(Qt::white);

        ip->setWhatsThis(JSON_ARRAY);
        domains->setWhatsThis(JSON_ARRAY);

        ip->setIcon(QIcon(":/img/res/icons/folder2.png"));
        domains->setIcon(QIcon(":/img/res/icons/folder2.png"));

        /* append to the NS */
        this->appendRow(ip);
        this->appendRow(domains);
    }
    ~NS()
    {
    }

public:
    QStandardItem *ip;
    QStandardItem *domains;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::NS &ns){
        this->setText(ns.ns);

        /* ip */
        int count = ip->rowCount();
        foreach(const QString &ip_address, ns.ip){
            ip->appendRow({new QStandardItem(QString::number(count)),
                                new QStandardItem(ip_address)});
            count++;
        }

        /* domains */
        count = domains->rowCount();
        foreach(const QString &domain, ns.domains){
            domains->appendRow({new QStandardItem(QString::number(count)),
                                new QStandardItem(domain)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};

}

s3s_struct::NS ns_to_struct(s3s_item::NS*);

QJsonObject ns_to_json(s3s_item::NS*);

void json_to_ns(const QJsonObject&, s3s_item::NS*);

#endif // NSITEM_H
