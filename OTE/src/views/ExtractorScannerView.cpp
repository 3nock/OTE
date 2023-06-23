//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExtractorScannerView.h"
#include "ui_ExtractorScannerView.h"

#include <QDebug>

#include "src/core/Config.h"
#include "src/engine/ExtractorEngine.h"
#include "src/dialogs/MultipleTargetsDialog.h"

ExtractorScannerView::ExtractorScannerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtractorScannerView)
{
    ui->setupUi(this);

    ui->comboBoxInputType->addItems(OTE::gInputOutput.inputTypes);
    ui->comboBoxOutputType->addItems(OTE::gInputOutput.outputTypes);

    ui->comboBoxInputType->setCurrentText("ip");
    ui->comboBoxOutputType->setCurrentText("asn");

    ui->buttonStop->setDisabled(true);
}

ExtractorScannerView::~ExtractorScannerView()
{
    delete ui;
}

void ExtractorScannerView::setLogsEdit(QPlainTextEdit *logsEdit)
{
    mLogsEdit = logsEdit;
}

void ExtractorScannerView::setSitemapView(ExtractorSitemapView *view)
{
    mSitemapView = view;
}

void ExtractorScannerView::setTemplatesView(ExtractorTemplatesView *view)
{
    mTemplatesView = view;
}

QString ExtractorScannerView::getInputType() const
{
    return ui->comboBoxInputType->currentText();
}

QString ExtractorScannerView::getOutputType() const
{
    return ui->comboBoxOutputType->currentText();
}

void ExtractorScannerView::onFinished()
{
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void ExtractorScannerView::onOutputTypeChanged(const QString &outputType)
{
    if(ui->comboBoxOutputType->currentText() == outputType)
        return;

    ui->comboBoxOutputType->setCurrentText(outputType);
}

void ExtractorScannerView::on_buttonMultipleTargets_clicked()
{
    QStringList targets{};

    QString description = tr("Enter target %1").arg(ui->comboBoxInputType->currentText());
    description.chop(1);
    description.append(" then start scan");

    MultipleTargetsDialog multipleTargetsDialog(&targets, description, this);
    if(multipleTargetsDialog.exec() == QDialog::Accepted)
        this->startScan(targets);
}

void ExtractorScannerView::on_buttonStart_clicked()
{
    QString target = ui->lineEditTarget->text();

    if(target.isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Target!"));
        return;
    }

    this->startScan({target});
}

void ExtractorScannerView::on_buttonStop_clicked()
{
    emit stop();
}

void ExtractorScannerView::on_comboBoxOutputType_currentTextChanged(const QString &type)
{
    if(type.isEmpty())
        return;

    emit outputTypeChanged(type);
}

void ExtractorScannerView::on_comboBoxInputType_currentTextChanged(const QString &type)
{
    if(type.isEmpty())
        return;

    ui->lineEditTarget->setPlaceholderText(tr("Enter target %1").arg(type));

    emit inputTypeChanged(type);
}

void ExtractorScannerView::startScan(const QStringList &targets)
{
    QList<OTE::Endpoint*> choosenEndpoints = mTemplatesView->getCheckedEnpoints();

    if(choosenEndpoints.isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Please select OSINT Templates to use!"));
        return;
    }

    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);

    QString inputType = ui->comboBoxInputType->currentText();
    QString outputType = ui->comboBoxOutputType->currentText();

    OTE::Engine::Extractor *engine = new OTE::Engine::Extractor(choosenEndpoints, inputType, outputType, targets);
    QThread *cThread = new QThread;
    engine->moveToThread(cThread);

    connect(this, &ExtractorScannerView::stop, engine, &OTE::Engine::Extractor::onStop);

    connect(engine, &OTE::Engine::Extractor::quitThread, cThread, &QThread::quit);
    connect(engine, &OTE::Engine::Extractor::result, mSitemapView, &ExtractorSitemapView::onResult);
    connect(engine, &OTE::Engine::Extractor::error, this, [=](const QString &link, const QString &error){
        QString log = QString("<b><font color=\"red\">Link: </font> </b> %1"
                              "<br>"
                              "<b><font color=\"red\">Error: </font> </b> %2"
                              "<br>").arg(link).arg(error);

        mLogsEdit->appendHtml(log);
    });

    connect(cThread, &QThread::started, engine, &OTE::Engine::Extractor::onStart);
    connect(cThread, &QThread::finished, this, &ExtractorScannerView::onFinished);
    connect(cThread, &QThread::finished, engine, &OTE::Engine::Extractor::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

    cThread->start();
}
