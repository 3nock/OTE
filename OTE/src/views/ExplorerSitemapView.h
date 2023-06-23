//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXPLORERSITEMAPVIEW_H
#define VIEWS_EXPLORERSITEMAPVIEW_H

#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QWidget>

#include "src/core/Highlighter.h"

namespace OTE {

namespace Explorer {

class ResultItem: public QStandardItem
{
public:
    ResultItem(const QString &tmplt, const QString &endpoint, const QString &target):
        QStandardItem(tmplt),
        endpointItem(new QStandardItem(endpoint)),
        targetItem(new QStandardItem(target))
    {}

    QStandardItem *endpointItem;
    QStandardItem *targetItem;
};

} // namespace Explorer

} // namespace OTE

namespace Ui {
class ExplorerSitemapView;
}

class ExplorerSitemapView : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerSitemapView(QWidget *parent = nullptr);
    ~ExplorerSitemapView();

public slots:
    void onClear();
    void onNewProject();

    void onResult(const QString &tmplt,
                  const QString &endPoint,
                  const QString &target);

private slots:
    void on_tableViewSitemap_clicked(const QModelIndex &index);
    void on_tableViewSitemap_customContextMenuRequested(const QPoint &pos);

private:
    Ui::ExplorerSitemapView *ui;

    QStandardItemModel *mModelSitemap;

    OTE::SyntaxHighlighter *mHighlighter = nullptr;
};

#endif // VIEWS_EXPLORERSITEMAPVIEW_H
