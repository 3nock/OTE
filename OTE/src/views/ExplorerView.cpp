//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExplorerView.h"
#include "ui_ExplorerView.h"

ExplorerView::ExplorerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerView),
    mExplorerScannerView(new ExplorerScannerView(this)),
    mExplorerSitemapView(new ExplorerSitemapView(this)),
    mExplorerTemplatesView(new ExplorerTemplatesView(this))
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.40))
                                        << static_cast<int>((this->width() * 0.60)));

    mExplorerScannerView->setSitemapView(mExplorerSitemapView);
    mExplorerScannerView->setLogsEdit(ui->plainTextEditLogs);

    connect(mExplorerTemplatesView, &ExplorerTemplatesView::templateEndpointChanged, mExplorerScannerView, &ExplorerScannerView::onTemplateEndpointChanged);

    ui->verticalLayout->insertWidget(0, mExplorerScannerView);
    ui->tabWidgetTemplate->insertTab(0, mExplorerTemplatesView, tr("Templates"));
    ui->tabWidgetSitemap->insertTab(0, mExplorerSitemapView, tr("Sitemap"));

    ui->tabWidgetTemplate->setCurrentIndex(0);
}

ExplorerView::~ExplorerView()
{
    delete mExplorerTemplatesView;
    delete mExplorerSitemapView;
    delete mExplorerScannerView;
    delete ui;
}

void ExplorerView::onClear()
{
    mExplorerSitemapView->onClear();
}

void ExplorerView::onNewProject()
{
    mExplorerSitemapView->onNewProject();
}

void ExplorerView::onKeysUpdated()
{
    mExplorerTemplatesView->onKeysUpdated();
}

void ExplorerView::onNewTemplate(OTE::Template *tmplt){
    mExplorerTemplatesView->onNewTemplate(tmplt);
}
