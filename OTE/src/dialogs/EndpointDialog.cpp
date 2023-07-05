//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "EndpointDialog.h"
#include "ui_EndpointDialog.h"

#include <QMessageBox>

#include "src/dialogs/ExtractorDialog.h"
#include "src/template/Extractor.h"

EndpointDialog::EndpointDialog(QWidget *parent, OTE::Endpoint *endpoint):
    QDialog(parent),
    ui(new Ui::EndpointDialog),
    mEndpoint(endpoint),
    mModelInputTypes(new QStringListModel),
    mModelExtrators(new QStandardItemModel)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("API Endpoint"));

    ui->lineEditEndpointName->setPlaceholderText(tr("e.g. Host search"));
    ui->plainTextEditEndpointDescription->setPlaceholderText(tr("e.g. Search Shodan using the same query syntax as the website and use facets to get summary information for different properties."));
    ui->lineEditEndpointLink->setPlaceholderText(tr("e.g. https://api.shodan.io/shodan/host/search?key={{key}}&query={{input}}"));
    ui->plainTextEditPostData->setPlaceholderText(tr("e.g. api_user={{user}}&api_key={{key}}&target={{input}}"));

    if(!mEndpoint->name.isEmpty())
        ui->lineEditEndpointName->setText(mEndpoint->name);

    if(!mEndpoint->link.isEmpty())
        ui->lineEditEndpointLink->setText(mEndpoint->link);

    if(!mEndpoint->postData.isEmpty())
        ui->plainTextEditPostData->setPlainText(mEndpoint->postData);

    if(!mEndpoint->description.isEmpty())
        ui->plainTextEditEndpointDescription->setPlainText(mEndpoint->description);

    mModelInputTypes->setStringList(mEndpoint->inputTypes);

    ui->labelPostData->hide();
    ui->plainTextEditPostData->hide();

    if(mEndpoint->requestType == OTE::Endpoint::REQUEST_TYPE::GET)
        ui->radioButtonGetReq->setChecked(true);
    else
        ui->radioButtonPostReq->setChecked(true);

    if(mEndpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::XML)
        ui->radioButtonXmlResp->setChecked(true);
    else
        ui->radioButtonJsonResp->setChecked(true);

    foreach(OTE::Extractor *extractor, mEndpoint->extractors)
        mModelExtrators->appendRow(new OTE::ExtractorItem(extractor));

    ui->tableView->setModel(mModelExtrators);

    ui->listViewInputTypes->setModel(mModelInputTypes);
}

EndpointDialog::~EndpointDialog()
{
    delete mModelExtrators;
    delete mModelInputTypes;
    delete ui;
}

void EndpointDialog::on_buttonOK_clicked()
{
    QString name = ui->lineEditEndpointName->text();
    QString link = ui->lineEditEndpointLink->text();
    QString description = ui->plainTextEditEndpointDescription->toPlainText();

    if(name.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter API Endpoint's name!"));
        return;
    }
    if(link.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter API Endpoint URL link!"));
        return;
    }
    if(description.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter API endpoint  description!"));
        return;
    }

    mEndpoint->name = name;
    mEndpoint->link = link;
    mEndpoint->description = description;
    mEndpoint->postData = ui->plainTextEditPostData->toPlainText();

    mEndpoint->inputTypes = mModelInputTypes->stringList();

    if(ui->radioButtonGetReq->isChecked())
        mEndpoint->requestType = OTE::Endpoint::REQUEST_TYPE::GET;
    else
        mEndpoint->requestType = OTE::Endpoint::REQUEST_TYPE::POST;

    if(ui->radioButtonXmlResp->isChecked())
        mEndpoint->responseType = OTE::Endpoint::RESPONSE_TYPE::XML;
    else
        mEndpoint->responseType = OTE::Endpoint::RESPONSE_TYPE::JSON;

    mEndpoint->extractors.clear();

    for(int i = 0; i < mModelExtrators->rowCount(); i++)
    {
        OTE::ExtractorItem *item = dynamic_cast<OTE::ExtractorItem*>(mModelExtrators->item(i, 0));
        mEndpoint->extractors << item->extractor;
    }

    accept();
}

void EndpointDialog::on_buttonAdd_clicked()
{
    OTE::Extractor *extractor = new OTE::Extractor(mEndpoint);
    ExtractorDialog extractorDialog(this, extractor);

    if(extractorDialog.exec() == Accepted)
        mModelExtrators->appendRow(new OTE::ExtractorItem(extractor));
    else
        delete extractor;
}

void EndpointDialog::on_buttonEdit_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(!selectionModel->isSelected(ui->tableView->currentIndex()))
        return;

    OTE::ExtractorItem *item = dynamic_cast<OTE::ExtractorItem*>(mModelExtrators->itemFromIndex(ui->tableView->currentIndex()));
    ExtractorDialog extractorDialog(this, item->extractor);
    extractorDialog.exec();
}

void EndpointDialog::on_buttonRemove_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if(!selectionModel->isSelected(ui->tableView->currentIndex()))
        return;

    mModelExtrators->removeRow(ui->tableView->currentIndex().row());
}

void EndpointDialog::on_buttonAddInputType_clicked()
{
    QString inputType = ui->lineEditInputType->text();
    if(inputType.isEmpty())
        return;

    QStringList list = mModelInputTypes->stringList();
    list << inputType;
    mModelInputTypes->setStringList(list);
    ui->lineEditInputType->clear();
}

void EndpointDialog::on_radioButtonGetReq_toggled(bool checked)
{
    if(checked)
    {
        ui->labelPostData->hide();
        ui->plainTextEditPostData->hide();
    }
}

void EndpointDialog::on_radioButtonPostReq_toggled(bool checked)
{
    if(checked)
    {
        ui->labelPostData->show();
        ui->plainTextEditPostData->show();
    }
}
