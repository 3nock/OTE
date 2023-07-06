//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExplorerScannerView.h"
#include "ui_ExplorerScannerView.h"

#include <QMessageBox>

#include "src/dialogs/MultipleTargetsDialog.h"

ExplorerScannerView::ExplorerScannerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerScannerView),
    mEngine(new OTE::Engine::Explorer)
{
    ui->setupUi(this);

    connect(this, &ExplorerScannerView::stop, mEngine, &OTE::Engine::Explorer::onStop);
    connect(mEngine, &OTE::Engine::Explorer::finished, this, &ExplorerScannerView::onFinished);

    ui->lineEditTarget->setDisabled(true);
    ui->buttonGet->setDisabled(true);
    ui->buttonStop->setDisabled(true);
    ui->buttonMultipleTargets->setDisabled(true);
}

ExplorerScannerView::~ExplorerScannerView()
{
    delete mEngine;
    delete ui;
}

void ExplorerScannerView::setLogsEdit(QPlainTextEdit *logsEdit)
{
    mLogsEdit = logsEdit;
    connect(mEngine, &OTE::Engine::Explorer::error, this, [=](const QString &link, const QString &error){
        QString log = QString("<b><font color=\"red\">Link: </font> </b> %1"
                              "<br>"
                              "<b><font color=\"red\">Error: </font> </b> %2"
                              "<br>").arg(link,error);

        mLogsEdit->appendHtml(log);
    });
}

void ExplorerScannerView::setSitemapView(ExplorerSitemapView *view)
{
    mExplorerSitemapView = view;
    connect(mEngine, &OTE::Engine::Explorer::result, mExplorerSitemapView, &ExplorerSitemapView::onResult);
}

void ExplorerScannerView::onTemplateEndpointChanged(OTE::Template *tmplt, OTE::Endpoint *endpoint)
{
    ui->lineEditTarget->clear();

    mCurrentTemplate = tmplt;
    mCurrentEndpoint = endpoint;

    if(mCurrentEndpoint == nullptr || mCurrentTemplate == nullptr)
    {
        ui->lineEditTarget->setPlaceholderText("");
        ui->lineEditTarget->setDisabled(true);
        ui->buttonGet->setDisabled(true);
        return;
    }
    else
    {
        ui->lineEditTarget->setEnabled(true);
        ui->buttonGet->setEnabled(true);
    }

    if(mCurrentEndpoint->inputTypes.length())
    {
        QString placeholderTxt(tr("Enter target "));
        foreach(const QString &type, endpoint->inputTypes)
            placeholderTxt.append(type).append("/");
        placeholderTxt.chop(1);

        ui->lineEditTarget->setPlaceholderText(placeholderTxt);
        ui->lineEditTarget->setEnabled(true);
        ui->buttonMultipleTargets->setEnabled(true);
    }
    else
    {
        ui->lineEditTarget->setPlaceholderText(tr("No target"));
        ui->lineEditTarget->setDisabled(true);
        ui->buttonMultipleTargets->setDisabled(true);
    }
}

void ExplorerScannerView::onFinished()
{
    ui->buttonGet->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void ExplorerScannerView::on_buttonGet_clicked()
{
    if(mCurrentEndpoint == nullptr || mCurrentTemplate == nullptr)
        return;

    if(mCurrentEndpoint->inputTypes.isEmpty())
    {
        this->startScan();
        return;
    }

    QString target = ui->lineEditTarget->text();
    if(target.isEmpty())
    {
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Target!"));
        return;
    }

    this->startScan({target});
}

void ExplorerScannerView::on_buttonStop_clicked()
{
    emit stop();
}

void ExplorerScannerView::on_buttonMultipleTargets_clicked()
{
    QStringList targets{};

    QString description("Enter target ");
    foreach(const QString &type, mCurrentEndpoint->inputTypes)
        description.append(type).append("/");
    description.chop(1);
    description.append(" then start scan");

    MultipleTargetsDialog multipleTargetsDialog(&targets, description, this);
    if(multipleTargetsDialog.exec() == QDialog::Accepted)
        this->startScan(targets);
}

void ExplorerScannerView::startScan()
{
    if((mCurrentTemplate->authentication.uses_id && mCurrentTemplate->authentication.id.isEmpty()) ||
       (mCurrentTemplate->authentication.uses_key && mCurrentTemplate->authentication.key.isEmpty()))
    {
        QMessageBox::warning(this, tr("No Key Error"), tr("This template requires API key but you haven't provided one!"));
        return;
    }

    ui->buttonGet->setDisabled(true);
    ui->buttonStop->setEnabled(true);

    mEngine->start(mCurrentEndpoint);
}

void ExplorerScannerView::startScan(const QStringList &targets)
{
    if((mCurrentTemplate->authentication.uses_id && mCurrentTemplate->authentication.id.isEmpty()) ||
       (mCurrentTemplate->authentication.uses_key && mCurrentTemplate->authentication.key.isEmpty()))
    {
        QMessageBox::warning(this, tr("No Key Error"), tr("This template requires API key but you haven't provided one!"));
        return;
    }

    ui->buttonGet->setDisabled(true);
    ui->buttonStop->setEnabled(true);

    mEngine->start(mCurrentEndpoint, targets);
}
