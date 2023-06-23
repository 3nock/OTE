//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "TemplateDialog.h"
#include "ui_TemplateDialog.h"

#include <QMessageBox>

#include "src/dialogs/EndpointDialog.h"
#include "src/template/Endpoint.h"

TemplateDialog::TemplateDialog(QWidget *parent, OTE::Template *_template):
    QDialog(parent),
    ui(new Ui::TemplateDialog),
    mTemplate(_template),
    mModelEndpoints(new QStandardItemModel)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("OSINT Template"));

    ui->lineEditAuthorName->setPlaceholderText("Enock N. M.");
    ui->lineEditAuthorSocial->setPlaceholderText("https://github.com/3nock");
    ui->lineEditAuthorName->setPlaceholderText(tr("e.g. Enock N. M."));
    ui->lineEditAuthorSocial->setPlaceholderText(tr("e.g. enock.n.michael@gmail.com"));
    ui->lineEditTemplateName->setPlaceholderText("e.g. Shodan");
    ui->lineEditTemplateWebsite->setPlaceholderText("e.g. https://shodan.io/");
    ui->lineEditTemplateDoc->setPlaceholderText(tr("e.g. https://shodan.io/api/doc"));
    ui->plainTextEditTemplateDescription->setPlaceholderText(tr("e.g. Shodan is an open source intelligence search engine."));
    ui->lineEditHeaderAuth->setPlaceholderText(tr("e.g. X-API-KEY"));
    ui->lineEditGetKey->setPlaceholderText(tr("e.g https://api.shodan.com/get_key"));

    if(!mTemplate->author.name.isEmpty())
        ui->lineEditAuthorName->setText(mTemplate->author.name);
    if(!mTemplate->author.social.isEmpty())
        ui->lineEditAuthorSocial->setText(mTemplate->author.social);

    if(!mTemplate->info.name.isEmpty())
        ui->lineEditTemplateName->setText(mTemplate->info.name);
    if(!mTemplate->info.link.isEmpty())
        ui->lineEditTemplateWebsite->setText(mTemplate->info.link);
    if(!mTemplate->info.documentation.isEmpty())
        ui->lineEditTemplateDoc->setText(mTemplate->info.documentation);
    if(!mTemplate->info.description.isEmpty())
        ui->plainTextEditTemplateDescription->setPlainText(mTemplate->info.description);

    if(mTemplate->authentication.uses_id)
        ui->checkBoxUser->setChecked(true);
    if(mTemplate->authentication.uses_key)
        ui->checkBoxKey->setChecked(true);

    if(!mTemplate->authentication.link.isEmpty())
        ui->lineEditGetKey->setText(mTemplate->authentication.link);

    if(!mTemplate->authentication.authHeader.isEmpty())
        ui->lineEditHeaderAuth->setText(mTemplate->authentication.authHeader);

    if(mTemplate->authentication.authType == OTE::Template::AUTH_TYPE::HEADER)
    {
        ui->radioButtonHeaderAuth->setChecked(true);
        ui->lineEditHeaderAuth->setText(mTemplate->authentication.authHeader);
    }
    else if(mTemplate->authentication.authType == OTE::Template::AUTH_TYPE::QUERY)
    {
        ui->radioButtonQueryAuth->setChecked(true);
        ui->labelHeaderAuth->hide();
        ui->lineEditHeaderAuth->hide();
    }
    else if(mTemplate->authentication.authType == OTE::Template::AUTH_TYPE::BASIC)
    {
        ui->radioButtonBasicAuth->setChecked(true);
        ui->labelHeaderAuth->hide();
        ui->lineEditHeaderAuth->hide();
    }
    else if(mTemplate->authentication.authType == OTE::Template::AUTH_TYPE::BEARER)
    {
        ui->radioButtonBearerAuth->setChecked(true);
        ui->labelHeaderAuth->hide();
        ui->lineEditHeaderAuth->hide();
    }
    else
    {
        ui->labelHeaderAuth->hide();
        ui->lineEditHeaderAuth->hide();
    }

    foreach(OTE::Endpoint *endpoint,  mTemplate->endpoints)
        mModelEndpoints->appendRow(new OTE::EndpointItem(endpoint));

    ui->tableViewEndpoints->setModel(mModelEndpoints);
}

TemplateDialog::~TemplateDialog()
{
    delete mModelEndpoints;
    delete ui;
}

void TemplateDialog::on_buttonSave_clicked()
{
    QString author_name = ui->lineEditAuthorName->text();
    QString author_link = ui->lineEditAuthorSocial->text();

    QString module_name = ui->lineEditTemplateName->text();
    QString module_link = ui->lineEditTemplateWebsite->text();
    QString module_doc = ui->lineEditTemplateDoc->text();
    QString module_description = ui->plainTextEditTemplateDescription->toPlainText();
    QString api_link = ui->lineEditGetKey->text();

    if(author_name.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter Author name!"));
        return;
    }
    if(author_link.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter Author's social!"));
        return;
    }
    if(module_name.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter Template name!"));
        return;
    }
    if(module_link.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter Template link!"));
        return;
    }
    if(module_doc.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter Template API documentation link!"));
        return;
    }
    if(module_description.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter Template description!"));
        return;
    }
    if((ui->checkBoxKey->isChecked() || ui->checkBoxUser->isChecked()) && api_link.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter link to Get API key"));
        return;
    }

    mTemplate->author.name = author_name;
    mTemplate->author.social = author_link;
    mTemplate->info.name = module_name;
    mTemplate->info.link = module_link;
    mTemplate->info.documentation = module_doc;
    mTemplate->info.description = module_description;

    mTemplate->authentication.uses_key = ui->checkBoxKey->isChecked();
    mTemplate->authentication.uses_id = ui->checkBoxUser->isChecked();
    mTemplate->authentication.link = api_link;
    mTemplate->authentication.authHeader = ui->lineEditHeaderAuth->text();

    if(ui->radioButtonBasicAuth->isChecked())
        mTemplate->authentication.authType = OTE::Template::AUTH_TYPE::BASIC;
    else if(ui->radioButtonBearerAuth->isChecked())
        mTemplate->authentication.authType = OTE::Template::AUTH_TYPE::BEARER;
    else if(ui->radioButtonHeaderAuth->isChecked())
        mTemplate->authentication.authType = OTE::Template::AUTH_TYPE::HEADER;
    else if(ui->radioButtonQueryAuth->isChecked())
        mTemplate->authentication.authType = OTE::Template::AUTH_TYPE::QUERY;

    mTemplate->endpoints.clear();
    for(int i = 0; i < mModelEndpoints->rowCount(); i++)
    {
        OTE::EndpointItem *item = static_cast<OTE::EndpointItem*>(mModelEndpoints->item(i, 0));
        mTemplate->endpoints << item->endpoint;
    }

    accept();
}

void TemplateDialog::on_buttonAddEndPoint_clicked()
{
    OTE::Endpoint *endpoint = new OTE::Endpoint(mTemplate);

    EndpointDialog endpointDialog(this, endpoint);
    if(endpointDialog.exec() == Accepted)
        mModelEndpoints->appendRow(new OTE::EndpointItem(endpoint));
    else
        delete endpoint;
}

void TemplateDialog::on_buttonEditEndPoint_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableViewEndpoints->selectionModel();
    if(!selectionModel->isSelected(ui->tableViewEndpoints->currentIndex()))
        return;

    OTE::EndpointItem *item = static_cast<OTE::EndpointItem*>(mModelEndpoints->itemFromIndex(ui->tableViewEndpoints->currentIndex()));
    EndpointDialog endpointDialog(this, item->endpoint);
    endpointDialog.exec();
}

void TemplateDialog::on_buttonRemoveEndPoint_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableViewEndpoints->selectionModel();
    if(!selectionModel->isSelected(ui->tableViewEndpoints->currentIndex()))
        return;

    mModelEndpoints->removeRow(ui->tableViewEndpoints->currentIndex().row());
}

void TemplateDialog::on_radioButtonQueryAuth_clicked(bool checked)
{
    if(checked)
    {
        ui->labelHeaderAuth->hide();
        ui->lineEditHeaderAuth->hide();
    }
}

void TemplateDialog::on_radioButtonHeaderAuth_clicked(bool checked)
{
    if(checked)
    {
        ui->labelHeaderAuth->show();
        ui->lineEditHeaderAuth->show();
    }
}

void TemplateDialog::on_radioButtonBasicAuth_clicked(bool checked)
{
    if(checked)
    {
        ui->labelHeaderAuth->hide();
        ui->lineEditHeaderAuth->hide();
    }
}

void TemplateDialog::on_radioButtonBearerAuth_clicked(bool checked)
{
    if(checked)
    {
        ui->labelHeaderAuth->hide();
        ui->lineEditHeaderAuth->hide();
    }
}
