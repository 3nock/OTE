//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXPLORERTEMPLATESVIEW_H
#define VIEWS_EXPLORERTEMPLATESVIEW_H

#include <QWidget>

#include "src/template/Endpoint.h"
#include "src/template/Template.h"

namespace Ui {
class ExplorerTemplatesView;
}

class ExplorerTemplatesView : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerTemplatesView(QWidget *parent = nullptr);
    ~ExplorerTemplatesView();

signals:
    void templateEndpointChanged(OTE::Template *tmplt, OTE::Endpoint *endpoint);

public slots:
    void onKeysUpdated();
    void onNewTemplate(OTE::Template *tmplt);

private slots:
    void on_lineEditFilter_textChanged(const QString &arg1);
    void on_tableViewTemplates_clicked(const QModelIndex &index);
    void on_tableViewTemplates_customContextMenuRequested(const QPoint &pos);
    void on_comboBoxEndpoints_currentIndexChanged(int index);

private:
    Ui::ExplorerTemplatesView *ui;

    QStandardItemModel *mModelTemplates;

    OTE::Template *mCurrentTemplate{};
    QList<OTE::TemplateItem*> mTemplateItems{};
};

#endif // VIEWS_EXPLORERTEMPLATESVIEW_H
