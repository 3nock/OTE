/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef MXMODEL_H
#define MXMODEL_H

#include <QStandardItem>
#include <QDate>
#include <QSet>


namespace s3s_struct {
struct MX{
    QString info_mx;
    QString info_ip;
    QSet<QString> domains;
};
}

namespace s3s_item {

class MX: public QStandardItem {
public:
    MX(): QStandardItem(),
        info(new QStandardItem("Info")),
        domains(new QStandardItem("Domains")),
        /* info */
        info_mx(new QStandardItem),
        info_ip(new QStandardItem)
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setForeground(Qt::white);
        domains->setForeground(Qt::white);

        info->setIcon(QIcon(":/img/res/icons/folder2.png"));
        domains->setIcon(QIcon(":/img/res/icons/folder2.png"));

        info->appendRow({new QStandardItem("MailServer"), info_mx});
        info->appendRow({new QStandardItem("Ip"), info_ip});

        /* append to the MX */
        this->appendRow(info);
        this->appendRow(domains);
    }
    ~MX(){}

public:
    QStandardItem *info;
    QStandardItem *domains;
    /* info */
    QStandardItem *info_mx;
    QStandardItem *info_ip;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::MX &mx){
        this->setText(mx.info_mx);

        /* info */
        info_mx->setText(mx.info_mx);
        info_ip->setText(mx.info_ip);

        /* domains */
        int count = domains->rowCount();
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

#endif // MXMODEL_H
