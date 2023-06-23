//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXTRACTORVIEW_H
#define VIEWS_EXTRACTORVIEW_H

#include <QWidget>

#include "src/template/Template.h"
#include "src/views/ExtractorScannerView.h"
#include "src/views/ExtractorSitemapView.h"
#include "src/views/ExtractorTemplatesView.h"

namespace Ui {
class ExtractorView;
}

class ExtractorView : public QWidget
{
    Q_OBJECT

public:
    explicit ExtractorView(QWidget *parent = nullptr);
    ~ExtractorView();

public slots:
    void onClear();
    void onNewProject();
    void onKeysUpdated();
    void onNewTemplate(OTE::Template *tmplt);

private:
    Ui::ExtractorView *ui;

    ExtractorScannerView *mExtractorScannerView;
    ExtractorSitemapView *mExtractorSitemapView;
    ExtractorTemplatesView *mExtractorTemplatesView;
};

#endif // VIEWS_EXTRACTORVIEW_H
