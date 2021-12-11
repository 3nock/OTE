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
