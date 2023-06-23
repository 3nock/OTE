//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExtractorView.h"
#include "ui_ExtractorView.h"

#include <QDebug>

ExtractorView::ExtractorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtractorView),
    mExtractorScannerView(new ExtractorScannerView(this)),
    mExtractorSitemapView(new ExtractorSitemapView(this)),
    mExtractorTemplatesView(new ExtractorTemplatesView(this))
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.40))
                                        << static_cast<int>((this->width() * 0.60)));

    mExtractorScannerView->setTemplatesView(mExtractorTemplatesView);
    mExtractorScannerView->setSitemapView(mExtractorSitemapView);
    mExtractorScannerView->setLogsEdit(ui->plainTextEditLogs);

    ui->verticalLayout->insertWidget(0, mExtractorScannerView);
    ui->tabWidgetTemplate->insertTab(0, mExtractorTemplatesView, tr("Templates"));
    ui->tabWidgetSitemap->insertTab(0, mExtractorSitemapView, tr("Sitemap"));

    connect(mExtractorScannerView, &ExtractorScannerView::inputTypeChanged, mExtractorTemplatesView, &ExtractorTemplatesView::onInputTypeChanged);
    connect(mExtractorScannerView, &ExtractorScannerView::outputTypeChanged, mExtractorTemplatesView, &ExtractorTemplatesView::onOutputTypeChanged);
    connect(mExtractorScannerView, &ExtractorScannerView::outputTypeChanged, mExtractorSitemapView, &ExtractorSitemapView::onOutputTypeChanged);

    connect(mExtractorSitemapView, &ExtractorSitemapView::outputTypeChanged, mExtractorScannerView, &ExtractorScannerView::onOutputTypeChanged);

    mExtractorTemplatesView->setInputOutputTypes(mExtractorScannerView->getInputType(),
                                                 mExtractorScannerView->getOutputType());
    ui->tabWidgetTemplate->setCurrentIndex(0);
}

ExtractorView::~ExtractorView()
{
    delete mExtractorTemplatesView;
    delete mExtractorSitemapView;
    delete mExtractorScannerView;
    delete ui;
}

void ExtractorView::onClear()
{
    mExtractorSitemapView->onClear();
}

void ExtractorView::onNewProject()
{
    mExtractorSitemapView->onNewProject();
}

void ExtractorView::onKeysUpdated()
{
    mExtractorTemplatesView->onKeysUpdated();
}

void ExtractorView::onNewTemplate(OTE::Template *tmplt)
{
    mExtractorTemplatesView->onNewTemplate(tmplt);
}
