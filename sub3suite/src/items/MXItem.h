/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef MXITEM_H
#define MXITEM_H

#include <QStandardItem>
#include <QDate>
#include <QSet>

#define JSON_ARRAY "arr"
#define JSON_OBJECT "obj"

namespace s3s_struct {
struct MX{
    QString mx;
    QSet<QString> ip;
    QSet<QString> domains;
};
}

namespace s3s_item {

class MX: public QStandardItem {
public:
    MX(): QStandardItem(),
        ip(new QStandardItem("IPs")),
        domains(new QStandardItem("Domains"))
    {
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        ip->setWhatsThis(JSON_ARRAY);
        domains->setWhatsThis(JSON_ARRAY);

        ip->setIcon(QIcon(":/img/res/icons/folder2.png"));
        domains->setIcon(QIcon(":/img/res/icons/folder2.png"));

        /* append to the MX */
        this->appendRow(ip);
        this->appendRow(domains);
    }
    ~MX(){}

public:
    QStandardItem *ip;
    QStandardItem *domains;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::MX &mx){
        this->setText(mx.mx);

        /* ip */
        int count = ip->rowCount();
        foreach(const QString &ip_address, mx.ip){
            ip->appendRow({new QStandardItem(QString::number(count)),
                                new QStandardItem(ip_address)});
            count++;
        }

        /* domains */
        count = domains->rowCount();
        foreach(const QString &domain, mx.domains){
            domains->appendRow({new QStandardItem(QString::number(count)),
                                new QStandardItem(domain)});
            count++;
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};

}

s3s_struct::MX mx_to_struct(s3s_item::MX*);

QJsonObject mx_to_json(s3s_item::MX*);

void json_to_mx(const QJsonObject&, s3s_item::MX*);

#endif // MXITEM_H
