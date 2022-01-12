/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef MXMODEL_H
#define MXMODEL_H

#include <QStandardItem>
#include <QSet>


/* structure for sending MXModel data along signals & slots */
struct MXModelStruct{
    QString info_mx;
    QString info_ip;
    QSet<QString> domains;
};

/* the main MX results data Model */
class MXModel{
public:
    MXModel():
        model(new QStandardItemModel),

        /* ... */
        info(new QStandardItem("Info")),
        domains(new QStandardItem("Domains")),

        /* info */
        info_mx(new QStandardItem),
        info_ip(new QStandardItem)
    {
        info->setForeground(QColor(220,220,220));
        domains->setForeground(QColor(220,220,220));

        QFont font("Segoe UI", 9, QFont::Bold);
        info->setFont(font);
        domains->setFont(font);

        info->appendRow({new QStandardItem("MailServer"), info_mx});
        info->appendRow({new QStandardItem("Ip"), info_ip});

        /* the model */
        model->setColumnCount(2);
        model->setHorizontalHeaderLabels({"  Property", "  Value"});

        /* append to the model */
        model->appendRow(info);
        model->appendRow(domains);
    }
    ~MXModel(){}

public:
    QStandardItemModel *model;

    /* ... */
    QStandardItem *info;
    QStandardItem *domains;

    /* info */
    QStandardItem *info_mx;
    QStandardItem *info_ip;
};

#endif // MXMODEL_H
