//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExtractorDialog.h"
#include "ui_ExtractorDialog.h"

#include <QDomDocument>
#include <QJsonDocument>
#include <QMessageBox>

#include "src/template/Endpoint.h"

ExtractorDialog::ExtractorDialog(QWidget *parent, OTE::Extractor *extractor) :
    QDialog(parent),
    ui(new Ui::ExtractorDialog),
    mExtractor(extractor)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Extractor"));

    ui->lineEditExtractorName->setPlaceholderText(tr("Extractor name e.g. subdomain"));
    ui->plainTextEditExtractor->setPlaceholderText(tr("Extraction script e.g. {\"data\":{subdomains:[\"$$\"]}}"));

    if(!mExtractor->name.isEmpty())
        ui->lineEditExtractorName->setText(mExtractor->name);

    if(!mExtractor->script.isEmpty())
    {
        QJsonDocument doc = QJsonDocument::fromJson(mExtractor->script);
        ui->plainTextEditExtractor->setPlainText(doc.toJson());
    }

    mHighlighter = new OTE::SyntaxHighlighter(ui->plainTextEditExtractor->document());

    if(extractor->endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::JSON)
        mHighlighter->setCurrentLanguage(OTE::SyntaxHighlighter::CodeJSON);
    else if(extractor->endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::XML)
        mHighlighter->setCurrentLanguage(OTE::SyntaxHighlighter::CodeXML);
}

ExtractorDialog::~ExtractorDialog()
{
    delete mHighlighter;
    delete ui;
}

void ExtractorDialog::on_buttonOK_clicked()
{
    QString name = ui->lineEditExtractorName->text();
    QByteArray script = ui->plainTextEditExtractor->toPlainText().toUtf8();

    if(script.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please enter the extractor name!"));
        return;
    }

    if(script.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please enter the extraction script!"));
        return;
    }

    mExtractor->name = name;

    if(OTE::Extractor::createExtractor(mExtractor, script))
        accept();
    else
        QMessageBox::warning(this, tr("Error"), tr("Please enter a valid extraction script!"));
}
