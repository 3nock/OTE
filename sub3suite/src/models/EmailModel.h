/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef EMAILMODEL_H
#define EMAILMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>


class EmailModel{
public:
    EmailModel():
        address(new QStandardItem),
        domain(new QStandardItem),
        free(new QStandardItem),
        disposable(new QStandardItem)
    {
    }
    ~EmailModel();
    QStandardItemModel *model;

public:
    /* ... */
    QStandardItem *address;
    QStandardItem *domain;
    QStandardItem *free;
    QStandardItem *disposable;
};

#endif // EMAILMODEL_H
