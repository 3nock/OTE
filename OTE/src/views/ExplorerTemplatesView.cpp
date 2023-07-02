//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExplorerTemplatesView.h"
#include "ui_ExplorerTemplatesView.h"

#include <QDebug>
#include <QMenu>

#include "src/dialogs/TemplateDialog.h"

Q_DECLARE_METATYPE(OTE::Endpoint*);

ExplorerTemplatesView::ExplorerTemplatesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerTemplatesView),
    mModelTemplates(new QStandardItemModel)
{
    ui->setupUi(this);

    ui->textBrowserTemplateDescription->setOpenExternalLinks(true);

    ui->lineEditFilter->setPlaceholderText(tr("Filter templates according to API endpoint's input type e.g. ip"));
    ui->textBrowserTemplateDescription->setPlaceholderText(tr("OSINT Template description"));
    ui->plainTextEditEndpointDescription->setPlaceholderText(tr("API Endpoint description"));

    mModelTemplates->setHorizontalHeaderLabels({tr("OSINT Templates")});

    foreach(OTE::Template *tmplt, OTE::gTemplates)
    {
        OTE::TemplateItem *item = new OTE::TemplateItem(tmplt);
        mModelTemplates->appendRow(item);
        mTemplateItems << item;
    }

    ui->tableViewTemplates->setModel(mModelTemplates);
}

ExplorerTemplatesView::~ExplorerTemplatesView()
{
    delete mModelTemplates;
    delete ui;
}

void ExplorerTemplatesView::onKeysUpdated()
{
    foreach(OTE::TemplateItem *item, mTemplateItems)
        item->configureIcon();
}

void ExplorerTemplatesView::onNewTemplate(OTE::Template *tmplt)
{
    OTE::TemplateItem *item = new OTE::TemplateItem(tmplt);
    mModelTemplates->appendRow(item);
    mTemplateItems << item;
}

void ExplorerTemplatesView::on_lineEditFilter_textChanged(const QString &pattern)
{
    // clear
    mCurrentTemplate = nullptr;
    ui->textBrowserTemplateDescription->clear();
    ui->plainTextEditEndpointDescription->clear();
    ui->comboBoxEndpoints->clear();

    mModelTemplates->takeColumn(0);
    while(mModelTemplates->rowCount())
        mModelTemplates->removeRow(0);

    if(pattern.isEmpty())
    {
        foreach(OTE::TemplateItem *item, mTemplateItems)
            mModelTemplates->appendRow(item);
        return;
    }

    foreach(OTE::TemplateItem *item, mTemplateItems)
    {
        foreach(OTE::Endpoint *endpoint, item->tmplt->endpoints)
        {
            if(endpoint->inputTypes.contains(pattern, Qt::CaseInsensitive))
            {
                mModelTemplates->appendRow(item);
                break;
            }
        }
    }
}

void ExplorerTemplatesView::on_tableViewTemplates_clicked(const QModelIndex &index)
{
    // clear
    mCurrentTemplate = nullptr;
    ui->comboBoxEndpoints->clear();
    ui->textBrowserTemplateDescription->clear();
    ui->plainTextEditEndpointDescription->clear();

    // obtain template item & set the details
    OTE::TemplateItem *item = static_cast<OTE::TemplateItem*>(mModelTemplates->itemFromIndex(index));

    QString info = QString("<b>"
                           "Template: <a href=\"%1\">%2</a>"
                           "<br>"
                           "API Documentation: <a href=\"%3\">%4</a>"
                           "</b>"
                           "<br>"
                           "<p>%5</p>").arg(item->tmplt->info.link)
                                       .arg(item->tmplt->info.name)\
                                       .arg(item->tmplt->info.documentation)\
                                       .arg(item->tmplt->info.documentation)\
                                       .arg(item->tmplt->info.description);

    ui->textBrowserTemplateDescription->append(info);

    // append all endpoints, if the filter is on, filter the endpoints accordingly
    foreach(OTE::Endpoint *endpoint, item->tmplt->endpoints)
    {
        QString filterPattern = ui->lineEditFilter->text();
        if(filterPattern.isEmpty())
        {
            QVariant v;
            v.setValue(endpoint);
            ui->comboBoxEndpoints->addItem(endpoint->name, v);
        }
        else
        {
            if(endpoint->inputTypes.contains(filterPattern, Qt::CaseInsensitive))
            {
                QVariant v;
                v.setValue(endpoint);
                ui->comboBoxEndpoints->addItem(endpoint->name, v);
            }
        }
    }

    mCurrentTemplate = item->tmplt;
    this->on_comboBoxEndpoints_currentIndexChanged(0);
}

void ExplorerTemplatesView::on_tableViewTemplates_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    QItemSelectionModel *selectionModel = ui->tableViewTemplates->selectionModel();

    // check if user right clicked on items else dont show the context menu...
    if(!selectionModel->isSelected(ui->tableViewTemplates->currentIndex()))
        return;

    // get item
    OTE::TemplateItem *item = static_cast<OTE::TemplateItem*>(mModelTemplates->itemFromIndex(ui->tableViewTemplates->currentIndex()));

    QMenu menu(this);

    menu.addAction(tr("Edit Template"), this, [=](){
        TemplateDialog templateDialog(this, item->tmplt);
        if(templateDialog.exec() == QDialog::Accepted)
            OTE::Template::SaveTemplate(item->tmplt);
    })->setIcon(QIcon(":/icons/res/icons/edit.png"));

    menu.exec(QCursor::pos());
}

void ExplorerTemplatesView::on_comboBoxEndpoints_currentIndexChanged(int index)
{
    ui->plainTextEditEndpointDescription->clear();

    if(mCurrentTemplate == nullptr)
    {
        emit templateEndpointChanged(nullptr, nullptr);
        return;
    }

    QVariant v = ui->comboBoxEndpoints->itemData(index);
    OTE::Endpoint *endpoint = v.value<OTE::Endpoint*>();

    ui->plainTextEditEndpointDescription->setPlainText(endpoint->description);

    emit templateEndpointChanged(mCurrentTemplate, endpoint);
}
