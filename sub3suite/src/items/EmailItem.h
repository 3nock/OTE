/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef EMAILITEM_H
#define EMAILITEM_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QJsonObject>
#include <QDate>

#define JSON_ARRAY "arr"
#define JSON_OBJECT "obj"

namespace s3s_struct {
struct Email {
    QString email;
    QString domain;
    bool free;
    bool hostExists;
    bool disposable;
    bool deliverable;
};
}

namespace s3s_item {
class Email: public QStandardItem {
public:
    Email(): QStandardItem (),
        info(new QStandardItem("Info")),
        domain(new QStandardItem),
        free(new QStandardItem),
        hostExists(new QStandardItem),
        disposable(new QStandardItem),
        deliverable(new QStandardItem)
    {
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        info->setIcon(QIcon(":/img/res/icons/folder2.png"));

        info->appendRow({new QStandardItem("Domain"), domain});
        info->appendRow({new QStandardItem("Free"), free});
        info->appendRow({new QStandardItem("Host Exists"), hostExists});
        info->appendRow({new QStandardItem("Disposable"), disposable});
        info->appendRow({new QStandardItem("Deliverable"), deliverable});

        info->setWhatsThis(JSON_OBJECT);

        this->appendRow(info);
    }
    ~Email()
    {
    }

public:
    QStandardItem *info;
    QStandardItem *domain;
    QStandardItem *free;
    QStandardItem *hostExists;
    QStandardItem *disposable;
    QStandardItem *deliverable;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::Email &email){
        this->setText(email.email);

        domain->setText(email.domain);

        if(email.free)
            free->setText("true");
        else
            free->setText("false");

        if(email.disposable)
            disposable->setText("true");
        else
            disposable->setText("false");

        if(email.deliverable)
            deliverable->setText("true");
        else
            deliverable->setText("false");

        if(email.hostExists)
            hostExists->setText("true");
        else
            hostExists->setText("false");

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};
}

s3s_struct::Email email_to_struct(s3s_item::Email*);

QJsonObject email_to_json(s3s_item::Email*);

void json_to_email(const QJsonObject&, s3s_item::Email*);

#endif // EMAILITEM_H
