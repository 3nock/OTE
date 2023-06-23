//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXTRACTORSCANNERVIEW_H
#define VIEWS_EXTRACTORSCANNERVIEW_H

#include <QPlainTextEdit>
#include <QWidget>

#include "src/template/Endpoint.h"
#include "src/template/Extractor.h"
#include "src/template/Template.h"
#include "src/views/ExtractorSitemapView.h"
#include "src/views/ExtractorTemplatesView.h"

namespace Ui {
class ExtractorScannerView;
}

class ExtractorScannerView : public QWidget
{
    Q_OBJECT

public:
    explicit ExtractorScannerView(QWidget *parent = nullptr);
    ~ExtractorScannerView();

    void setLogsEdit(QPlainTextEdit *logsEdit);
    void setSitemapView(ExtractorSitemapView *view);
    void setTemplatesView(ExtractorTemplatesView *view);

    QString getInputType() const;
    QString getOutputType() const;

signals:
    void stop();

    void inputTypeChanged(const QString &inputType);
    void outputTypeChanged(const QString &outputType);

public slots:
    void onFinished();

    void onOutputTypeChanged(const QString &outputType);

private slots:
    void on_buttonMultipleTargets_clicked();
    void on_buttonStart_clicked();
    void on_buttonStop_clicked();
    void on_comboBoxOutputType_currentTextChanged(const QString &type);
    void on_comboBoxInputType_currentTextChanged(const QString &type);

private:
    Ui::ExtractorScannerView *ui;

    QPlainTextEdit *mLogsEdit = nullptr;
    ExtractorSitemapView *mSitemapView = nullptr;
    ExtractorTemplatesView *mTemplatesView = nullptr;

    void startScan(const QStringList &targets);
};

#endif // VIEWS_EXTRACTORSCANNERVIEW_H
