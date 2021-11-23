#ifndef EMAILMODEL_H
#define EMAILMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class EmailModel{
public:
    EmailModel(){}
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
