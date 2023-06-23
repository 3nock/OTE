//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXPLORERSCANNERVIEW_H
#define VIEWS_EXPLORERSCANNERVIEW_H

#include <QPlainTextEdit>
#include <QWidget>

#include "src/engine/ExplorerEngine.h"
#include "src/template/Endpoint.h"
#include "src/template/Template.h"
#include "src/views/ExplorerSitemapView.h"

namespace Ui {
class ExplorerScannerView;
}

class ExplorerScannerView : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerScannerView(QWidget *parent = nullptr);
    ~ExplorerScannerView();

    void setLogsEdit(QPlainTextEdit *logsEdit);
    void setSitemapView(ExplorerSitemapView *view);

signals:
    void stop();

public slots:
    void onTemplateEndpointChanged(OTE::Template *tmplt, OTE::Endpoint *endpoint);
    void onFinished();

private slots:
    void on_buttonGet_clicked();
    void on_buttonStop_clicked();
    void on_buttonMultipleTargets_clicked();

private:
    Ui::ExplorerScannerView *ui;

    OTE::Template *mCurrentTemplate = nullptr;
    OTE::Endpoint *mCurrentEndpoint = nullptr;

    QPlainTextEdit *mLogsEdit = nullptr;
    ExplorerSitemapView *mExplorerSitemapView = nullptr;

    OTE::Engine::Explorer *mEngine;

    void startScan();
    void startScan(const QStringList &targets);
};

#endif // VIEWS_EXPLORERSCANNERVIEW_H
