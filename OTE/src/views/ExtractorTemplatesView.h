//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXTRACTORTEMPLATESVIEW_H
#define VIEWS_EXTRACTORTEMPLATESVIEW_H

#include <QStandardItemModel>
#include <QWidget>

#include "src/template/Endpoint.h"
#include "src/template/Template.h"

namespace Ui {
class ExtractorTemplatesView;
}

class ExtractorTemplatesView : public QWidget
{
    Q_OBJECT

public:
    explicit ExtractorTemplatesView(QWidget *parent = nullptr);
    ~ExtractorTemplatesView();

    void setInputOutputTypes(const QString &input, const QString &output);
    QList<OTE::Endpoint*> getCheckedEnpoints();

public slots:
    void onKeysUpdated();
    void onNewTemplate(OTE::Template *tmplt);

    void onInputTypeChanged(const QString &inputType);
    void onOutputTypeChanged(const QString &outputType);

private slots:
    void on_treeViewTemplates_customContextMenuRequested(const QPoint &pos);
    void on_treeViewTemplates_clicked(const QModelIndex &index);

private:
    Ui::ExtractorTemplatesView *ui;

    QStandardItemModel *mModelTemplates;
    QList<OTE::TemplateItem*> mTemplateItems;

    QString mCurrentInputType;
    QString mCurrentOutputType;
};

#endif // VIEWS_EXTRACTORTEMPLATESVIEW_H
