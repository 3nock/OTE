/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef EMAILMODEL_H
#define EMAILMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QJsonObject>
#include <QDate>


namespace s3s_struct {
struct Email {
    QString email;
    QString address;
    QString domain;
    bool free;
    bool disposable;
};
}

namespace s3s_item {
class Email: public QStandardItem {
public:
    Email(): QStandardItem (),
        info(new QStandardItem("Info")),
        address(new QStandardItem()),
        domain(new QStandardItem),
        free(new QStandardItem),
        disposable(new QStandardItem)
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setForeground(Qt::white);
        info->setIcon(QIcon(":/img/res/icons/folder2.png"));

        info->appendRow({new QStandardItem("Address"), address});
        info->appendRow({new QStandardItem("Domain"), domain});
        info->appendRow({new QStandardItem("Free"), free});
        info->appendRow({new QStandardItem("Disposable"), disposable});

        this->appendRow(info);
    }
    ~Email()
    {
    }

public:
    QStandardItem *info;
    QStandardItem *address;
    QStandardItem *domain;
    QStandardItem *free;
    QStandardItem *disposable;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::Email &email){
        this->setText(email.email);

        address->setText(email.address);
        domain->setText(email.domain);

        if(email.free)
            free->setText("true");
        else
            free->setText("false");

        if(email.disposable)
            disposable->setText("true");
        else
            disposable->setText("false");

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};
}

s3s_struct::Email email_to_struct(s3s_item::Email*);

QJsonObject email_to_json(s3s_item::Email*);

void json_to_email(const QJsonObject&, s3s_item::Email*);

#endif // EMAILMODEL_H
