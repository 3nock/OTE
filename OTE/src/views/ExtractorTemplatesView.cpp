//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExtractorTemplatesView.h"
#include "ui_ExtractorTemplatesView.h"

#include <QDebug>
#include <QMenu>

#include "src/dialogs/TemplateDialog.h"

ExtractorTemplatesView::ExtractorTemplatesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtractorTemplatesView),
    mModelTemplates(new QStandardItemModel)
{
    ui->setupUi(this);

    ui->textBrowserDescription->setOpenExternalLinks(true);
    ui->textBrowserDescription->setPlaceholderText(tr("Template / API Endpoint description."));

    mModelTemplates->setHorizontalHeaderLabels({tr("OSINT Templates")});

    foreach(OTE::Template *tmplt, OTE::gTemplates)
    {
        if(tmplt->hasExtractors())
        {
            OTE::TemplateItem *item = new OTE::TemplateItem(tmplt);
            item->configureExtractors();
            mTemplateItems << item;
        }
    }

    ui->treeViewTemplates->setModel(mModelTemplates);
    ui->treeViewTemplates->header()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

ExtractorTemplatesView::~ExtractorTemplatesView()
{
    delete mModelTemplates;
    delete ui;
}

void ExtractorTemplatesView::setInputOutputTypes(const QString &input, const QString &output)
{
    mCurrentInputType = input;
    mCurrentOutputType = output;

    mModelTemplates->takeColumn(0);
    while(mModelTemplates->rowCount())
        mModelTemplates->removeRow(0);
    mModelTemplates->setHorizontalHeaderLabels({tr("OSINT Templates")});

    foreach(OTE::TemplateItem *item, mTemplateItems)
    {
        if(item->configureExtractors(input, output))
            mModelTemplates->appendRow(item);
    }

    ui->treeViewTemplates->expandAll();
}

QList<OTE::Endpoint *> ExtractorTemplatesView::getCheckedEnpoints()
{
    QList<OTE::Endpoint *> checkedEndpoints;
    foreach(OTE::TemplateItem *item, mTemplateItems)
    {
        QList<OTE::Endpoint *> endpoints = item->getCheckedEndpoints();
        if(endpoints.isEmpty())
            continue;

        checkedEndpoints << endpoints;
    }

    return checkedEndpoints;
}

void ExtractorTemplatesView::onKeysUpdated()
{
    foreach(OTE::TemplateItem *item, mTemplateItems)
        item->configureIcon();
}

void ExtractorTemplatesView::onNewTemplate(OTE::Template *tmplt)
{
    if(tmplt->hasExtractors())
    {
        auto *item = new OTE::TemplateItem(tmplt);
        mTemplateItems << item;
    }
}

void ExtractorTemplatesView::onInputTypeChanged(const QString &inputType)
{
    if(inputType == mCurrentInputType)
        return;
    else
        mCurrentInputType = inputType;

    ui->textBrowserDescription->clear();

    mModelTemplates->takeColumn(0);
    while(mModelTemplates->rowCount())
        mModelTemplates->removeRow(0);
    mModelTemplates->setHorizontalHeaderLabels({tr("OSINT Templates")});

    foreach(OTE::TemplateItem *item, mTemplateItems)
    {
        if(item->configureExtractors(mCurrentInputType, mCurrentOutputType))
            mModelTemplates->appendRow(item);
    }

    ui->treeViewTemplates->expandAll();
}

void ExtractorTemplatesView::onOutputTypeChanged(const QString &outputType)
{
    if(outputType == mCurrentOutputType)
        return;
    else
        mCurrentOutputType = outputType;

    ui->textBrowserDescription->clear();

    mModelTemplates->takeColumn(0);
    while(mModelTemplates->rowCount())
        mModelTemplates->removeRow(0);
    mModelTemplates->setHorizontalHeaderLabels({tr("OSINT Templates")});

    foreach(OTE::TemplateItem *item, mTemplateItems)
    {
        if(item->configureExtractors(mCurrentInputType, mCurrentOutputType))
            mModelTemplates->appendRow(item);
    }

    ui->treeViewTemplates->expandAll();
}

void ExtractorTemplatesView::on_treeViewTemplates_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    QItemSelectionModel *selectionModel = ui->treeViewTemplates->selectionModel();

    // check if user right clicked on items else dont show the context menu...
    if(!selectionModel->isSelected(ui->treeViewTemplates->currentIndex()))
        return;

    // get item
    OTE::TemplateItem *item = static_cast<OTE::TemplateItem*>(mModelTemplates->itemFromIndex(ui->treeViewTemplates->currentIndex()));

    QMenu menu(this);

    menu.addAction(tr("Edit Template"), this, [=](){
        TemplateDialog templateDialog(this, item->tmplt);
        if(templateDialog.exec() == QDialog::Accepted)
            OTE::Template::SaveTemplate(item->tmplt);
    });

    menu.exec(QCursor::pos());
}

void ExtractorTemplatesView::on_treeViewTemplates_clicked(const QModelIndex &index)
{
    ui->textBrowserDescription->clear();
    QStandardItem *item = mModelTemplates->itemFromIndex(index);

    if(item->hasChildren())
    {
        OTE::TemplateItem *templateItem = static_cast<OTE::TemplateItem*>(item);
        QString info = QString("<b>"
                               "Template: <a href=\"%1\">%2</a>"
                               "<br>"
                               "API Documentation: <a href=\"%3\">%4</a>"
                               "</b>"
                               "<br>"
                               "<p>%5</p>").arg(templateItem->tmplt->info.link)
                                           .arg(templateItem->tmplt->info.name)\
                                           .arg(templateItem->tmplt->info.documentation)\
                                           .arg(templateItem->tmplt->info.documentation)\
                                           .arg(templateItem->tmplt->info.description);
        ui->textBrowserDescription->setText(info);
    }
    else
    {
        OTE::EndpointItem *endpointItem = static_cast<OTE::EndpointItem*>(item);
        ui->textBrowserDescription->setText(endpointItem->endpoint->description);
    }
}