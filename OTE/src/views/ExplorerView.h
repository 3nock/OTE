//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXPLORERVIEW_H
#define VIEWS_EXPLORERVIEW_H

#include <QWidget>

#include "src/views/ExplorerScannerView.h"
#include "src/views/ExplorerSitemapView.h"
#include "src/views/ExplorerTemplatesView.h"

namespace Ui {
class ExplorerView;
}

class ExplorerView : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerView(QWidget *parent = nullptr);
    ~ExplorerView();

public slots:
    void onClear();
    void onNewProject();
    void onKeysUpdated();
    void onNewTemplate(OTE::Template *tmplt);

private:
    Ui::ExplorerView *ui;

    ExplorerScannerView *mExplorerScannerView;
    ExplorerSitemapView *mExplorerSitemapView;
    ExplorerTemplatesView *mExplorerTemplatesView;
};

#endif // VIEWS_EXPLORERVIEW_H
